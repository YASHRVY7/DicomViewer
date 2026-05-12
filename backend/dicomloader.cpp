#include "dicomloader.h"

#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmimgle/dcmimage.h>

#include <QDebug>

DicomLoader::DicomLoader(QObject *parent)
    : QObject{parent}
{}

bool DicomLoader::loadFile(const QString &filePath){
    DcmFileFormat fileFormat;
    OFCondition status = fileFormat.loadFile(filePath.toStdString().c_str());

    if(status.bad()){
        qWarning() << "Failed to load DICOM file:" << filePath;
        return false;
    }
    DcmDataset* dataset = fileFormat.getDataset();
    OFString value;


    if (dataset->findAndGetOFString(DCM_PatientName, value).good())
        m_patientName = QString::fromStdString(value.c_str());
    else
        m_patientName = "Unknown";

    if (dataset->findAndGetOFString(DCM_StudyDate, value).good())
        m_studyDate = QString::fromStdString(value.c_str());
    else
        m_studyDate = "Unknown";


    DicomImage image(filePath.toStdString().c_str());

    if (image.getStatus() != EIS_Normal) {
        qWarning() << "Invalid DICOM image";
        return false;
    }

    int w = image.getWidth();
    int h = image.getHeight();

    if (w <= 0 || h <= 0) return false;

    QImage dicomImage(w, h, QImage::Format_Grayscale8);

    image.setMinMaxWindow();
    image.getOutputData(dicomImage.bits(),
                        dicomImage.sizeInBytes(), 8);

    m_image = dicomImage;
    return true;
}
QImage  DicomLoader::currentImage() const {
    return m_image;
}

QString DicomLoader::patientName()  const {
    return m_patientName;
}

QString DicomLoader::studyDate()    const {
    return m_studyDate;
}
