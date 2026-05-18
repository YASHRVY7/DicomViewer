#include "dicomloader.h"

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmimage/diregist.h>
#include <dcmtk/dcmjpeg/djdecode.h>

#include <QDir>
#include <QFileInfo>
#include <QByteArray>

#include <cstring>

#include <QDebug>

static QString readTag(DcmDataset* dataset,
                       const DcmTagKey& tag)
{
    OFString value;

    if (dataset->findAndGetOFString(tag, value).good())
        return QString::fromStdString(value.c_str());

    return "Unknown";
}

namespace {

QImage renderDicomToQImage(DicomImage &image, double windowWidth,double windowLevel)
{
    if (image.getStatus() != EIS_Normal)
        return {};

    const int w = image.getWidth();
    const int h = image.getHeight();
    if (w <= 0 || h <= 0)
        return {};

    image.setWindow(
        windowWidth,
        windowLevel
    );

    const unsigned long outBytes = image.getOutputDataSize(8);
    if (outBytes == 0) {
        qWarning() << "DCMTK getOutputDataSize(8) returned 0";
        return {};
    }

    QByteArray buf(static_cast<int>(outBytes), Qt::Uninitialized);
    if (!image.getOutputData(buf.data(), outBytes, 8)) {
        qWarning() << "DCMTK getOutputData failed; need" << outBytes << "bytes for" << w << "x" << h;
        return {};
    }

    const auto uw = static_cast<unsigned long>(w);
    const auto uh = static_cast<unsigned long>(h);
    const unsigned long monoPacked = uw * uh;
    const unsigned long rgbPacked = monoPacked * 3u;

    if (outBytes == monoPacked) {
        QImage qimg(w, h, QImage::Format_Grayscale8);
        for (int y = 0; y < h; ++y) {
            memcpy(qimg.scanLine(y),
                   buf.constData() + static_cast<size_t>(y) * uw,
                   static_cast<size_t>(w));
        }
        return qimg;
    }

    if (outBytes == rgbPacked) {
        QImage qimg(w, h, QImage::Format_RGB888);
        const size_t srcStride = static_cast<size_t>(w) * 3u;
        for (int y = 0; y < h; ++y)
            memcpy(qimg.scanLine(y), buf.constData() + static_cast<size_t>(y) * srcStride, srcStride);
        return qimg;
    }

    if (uh != 0 && outBytes % uh == 0) {
        const unsigned long rowPitch = outBytes / uh;
        if (rowPitch >= uw * 3u) {
            QImage qimg(w, h, QImage::Format_RGB888);
            const size_t n = static_cast<size_t>(w) * 3u;
            for (int y = 0; y < h; ++y) {
                memcpy(qimg.scanLine(y),
                       buf.constData() + static_cast<size_t>(y) * rowPitch,
                       n);
            }
            return qimg;
        }
        if (rowPitch >= uw) {
            QImage qimg(w, h, QImage::Format_Grayscale8);
            for (int y = 0; y < h; ++y) {
                memcpy(qimg.scanLine(y),
                       buf.constData() + static_cast<size_t>(y) * rowPitch,
                       static_cast<size_t>(w));
            }
            return qimg;
        }
    }

    const unsigned long out16 = image.getOutputDataSize(16);
    if (out16 == monoPacked * 2u) {
        QByteArray buf16(static_cast<int>(out16), Qt::Uninitialized);
        if (image.getOutputData(buf16.data(), out16, 16)) {
            QImage qimg(w, h, QImage::Format_Grayscale16);
            for (int y = 0; y < h; ++y) {
                memcpy(qimg.scanLine(y),
                       buf16.constData() + static_cast<size_t>(y) * uw * 2u,
                       static_cast<size_t>(w) * 2u);
            }
            return qimg;
        }
    }

    qWarning() << "Unexpected DCMTK frame layout: size" << outBytes << "for" << w << "x" << h;
    return {};
}

} // namespace

DicomLoader::DicomLoader(QObject *parent)
    : QObject(parent)
{
    DJDecoderRegistration::registerCodecs();
}
DicomLoader::~DicomLoader()
{
    DJDecoderRegistration::cleanup();
}


bool DicomLoader::loadFolder(const QString &folderPath)
{
    m_images.clear();
    m_filePaths.clear();
    m_currentIndex = 0;
    m_imageWidth = 0;
    m_imageHeight = 0;

    QFileInfo fileInfo(folderPath);

    qDebug() << "Path:" << folderPath;
    qDebug() << "Is file:" << fileInfo.isFile();
    qDebug() << "Is dir:" << fileInfo.isDir();

    // If it's a file, load it directly
    if (fileInfo.isFile()) {
        qDebug() << "Loading single file:" << folderPath;
        return loadFile(folderPath);
    }

    QDir folder(folderPath);

    qDebug() << "Attempting to load folder:" << folderPath;
    qDebug() << "Folder exists:" << folder.exists();
    qDebug() << "Folder is readable:" << folder.isReadable();

    // Support more DICOM file extensions
    QStringList files =
        folder.entryList(QDir::Files | QDir::NoDotAndDotDot);

    qDebug() << "Found" << files.size() << "DICOM files";

    if (files.isEmpty()) {
        qWarning() << "No DICOM files found in folder:" << folderPath;
        // List all files for debugging
        QStringList allFiles = folder.entryList(QDir::Files);
        qDebug() << "All files in folder:" << allFiles;
        return false;
    }

    for (const QString& file : files) {

        QString fullPath = folder.absoluteFilePath(file);

        qDebug() << "Loading file:" << fullPath;

        DcmFileFormat fileFormat;

        // Load file with flags to handle compressed data
        OFCondition status =
            fileFormat.loadFile(fullPath.toStdString().c_str());

        if (status.bad()) {
            qWarning() << "Failed to load DICOM file:" << file << status.text();
            continue;
        }

        DcmDataset* dataset = fileFormat.getDataset();

        m_patientName =
            readTag(dataset, DCM_PatientName);

        m_studyDate =
            readTag(dataset, DCM_StudyDate);

        m_patientId =
            readTag(dataset, DCM_PatientID);

        m_modality =
            readTag(dataset, DCM_Modality);

        m_studyDescription =
            readTag(dataset, DCM_StudyDescription);

        m_seriesDescription =
            readTag(dataset, DCM_SeriesDescription);

        m_institutionName =
            readTag(dataset, DCM_InstitutionName);

        m_sliceThickness =
            readTag(dataset, DCM_SliceThickness);

        m_pixelSpacing =
            readTag(dataset, DCM_PixelSpacing);

        // Create DicomImage with flags to handle compressed data and missing PhotometricInterpretation
        unsigned long flags = 0;
        DicomImage image(fullPath.toStdString().c_str(), flags);

        if (image.getStatus() != EIS_Normal) {
            qWarning() << "Failed to create DicomImage for:" << file << "Status:" << image.getStatus();
            continue;
        }

        const QImage dicomImage = renderDicomToQImage(image,m_windowWidth,m_windowLevel);
        if (dicomImage.isNull()) {
            qWarning() << "Failed to decode pixels for:" << file;
            continue;
        }

        if (m_imageWidth == 0 && m_imageHeight == 0) {
            m_imageWidth = dicomImage.width();
            m_imageHeight = dicomImage.height();
        }

        m_images.append(dicomImage);
        m_filePaths.append(fullPath);
        qDebug() << "Successfully loaded DICOM file:" << file;
    }

    qDebug() << "Loaded" << m_images.size() << "DICOM images";
    return !m_images.isEmpty();
}

bool DicomLoader::loadFile(const QString &filePath)
{
    m_images.clear();
    m_filePaths.clear();
    m_currentIndex = 0;
    m_imageWidth = 0;
    m_imageHeight = 0;

    qDebug() << "Loading DICOM file:" << filePath;

    DcmFileFormat fileFormat;

    OFCondition status = fileFormat.loadFile(filePath.toStdString().c_str());

    if(status.bad()){
        qWarning() << "Failed to load DICOM file:" << filePath << status.text();
        return false;
    }

    DcmDataset* dataset = fileFormat.getDataset();

    m_patientName =
        readTag(dataset, DCM_PatientName);

    m_studyDate =
        readTag(dataset, DCM_StudyDate);

    m_patientId =
        readTag(dataset, DCM_PatientID);

    m_modality =
        readTag(dataset, DCM_Modality);

    m_studyDescription =
        readTag(dataset, DCM_StudyDescription);

    m_seriesDescription =
        readTag(dataset, DCM_SeriesDescription);

    m_institutionName =
        readTag(dataset, DCM_InstitutionName);

    m_sliceThickness =
        readTag(dataset, DCM_SliceThickness);

    m_pixelSpacing =
        readTag(dataset, DCM_PixelSpacing);

    // Try to load with different flags to handle compressed data
    DicomImage *image = nullptr;

    // First try with default flags
    image = new DicomImage(filePath.toStdString().c_str());

    if (image->getStatus() != EIS_Normal) {
        qWarning() << "Failed with default flags, Status:" << image->getStatus();
        delete image;

        // Try with force reading pixel data
        unsigned long flags = 0;
        image = new DicomImage(filePath.toStdString().c_str(), flags);

        if (image->getStatus() != EIS_Normal) {
            qWarning() << "Failed with flags=0, Status:" << image->getStatus();
            delete image;
            return false;
        }
    }

    const QImage dicomImage = renderDicomToQImage(*image,m_windowWidth,m_windowLevel);
    delete image;

    if (dicomImage.isNull()) {
        qWarning() << "Failed to render DICOM pixels for:" << filePath;
        return false;
    }

    m_imageWidth = dicomImage.width();
    m_imageHeight = dicomImage.height();

    m_image = dicomImage;
    m_images.append(dicomImage);
    m_filePaths.append(filePath);

    qDebug() << "Successfully loaded DICOM file:" << filePath;
    return true;
}


// void DicomLoader::rerenderCurrentImage()
// {
//     if (m_filePaths.isEmpty())
//         return;

//     if (m_currentIndex < 0 ||
//         m_currentIndex >= m_filePaths.size())
//         return;

//     QString filePath =
//         m_filePaths[m_currentIndex];

//     DicomImage image(
//         filePath.toStdString().c_str()
//         );

//     if (image.getStatus() != EIS_Normal) {

//         unsigned long flags = 0;

//         DicomImage imageWithFlags(
//             filePath.toStdString().c_str(),
//             flags
//             );

//         if (imageWithFlags.getStatus() != EIS_Normal)
//             return;

//         QImage newImage =
//             renderDicomToQImage(
//                 imageWithFlags,
//                 m_windowWidth,
//                 m_windowLevel
//                 );

//         if (!newImage.isNull()) {
//             m_images[m_currentIndex] = newImage;
//             m_image = newImage;
//         }

//         return;
//     }

//     QImage newImage =
//         renderDicomToQImage(
//             image,
//             m_windowWidth,
//             m_windowLevel
//             );

//     if (!newImage.isNull()) {
//         m_images[m_currentIndex] = newImage;
//         m_image = newImage;
//     }
// }
void DicomLoader::rerenderAllImages()
{
    if (m_filePaths.isEmpty())
        return;

    for (int i = 0; i < m_filePaths.size(); i++) {

        QString filePath =
            m_filePaths[i];

        DicomImage image(
            filePath.toStdString().c_str()
            );

        if (image.getStatus() != EIS_Normal)
            continue;

        QImage newImage =
            renderDicomToQImage(
                image,
                m_windowWidth,
                m_windowLevel
                );

        if (!newImage.isNull())
            m_images[i] = newImage;
    }

    if (m_currentIndex >= 0 &&
        m_currentIndex < m_images.size()) {

        m_image =
            m_images[m_currentIndex];
    }
}

QImage DicomLoader::currentImage() const
{
    if (m_images.isEmpty())
        return QImage();

    return m_images[m_currentIndex];
}

QString DicomLoader::patientName() const
{
    return m_patientName;
}

QString DicomLoader::studyDate() const
{
    return m_studyDate;
}

void DicomLoader::nextSlice()
{
    if (m_currentIndex < m_images.size() - 1)
        m_currentIndex++;
}

void DicomLoader::previousSlice()
{
    if (m_currentIndex > 0)
        m_currentIndex--;
}

void DicomLoader::goToIndex(int index)
{
    if (m_images.isEmpty())
        return;
    const int last = m_images.size() - 1;
    if (index < 0)
        m_currentIndex = 0;
    else if (index > last)
        m_currentIndex = last;
    else
        m_currentIndex = index;
}

int DicomLoader::currentIndex() const
{
    return m_currentIndex;
}

int DicomLoader::totalSlices() const
{
    return m_images.size();
}
void DicomLoader::setWindowWidth(double width)
{
    m_windowWidth = width;
}

void DicomLoader::setWindowLevel(double level)
{
    m_windowLevel = level;
}

double DicomLoader::windowWidth() const
{
    return m_windowWidth;
}

double DicomLoader::windowLevel() const
{
    return m_windowLevel;
}

QString DicomLoader::patientId() const
{
    return m_patientId;
}

QString DicomLoader::modality() const
{
    return m_modality;
}

QString DicomLoader::studyDescription() const
{
    return m_studyDescription;
}

QString DicomLoader::seriesDescription() const
{
    return m_seriesDescription;
}

QString DicomLoader::institutionName() const
{
    return m_institutionName;
}

QString DicomLoader::sliceThickness() const
{
    return m_sliceThickness;
}

QString DicomLoader::pixelSpacing() const
{
    return m_pixelSpacing;
}

int DicomLoader::imageWidth() const
{
    return m_imageWidth;
}

int DicomLoader::imageHeight() const
{
    return m_imageHeight;
}
