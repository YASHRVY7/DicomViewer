#include "dicomcontroller.h"
#include "dicomimageprovider.h"
#include "dicomloader.h"

#include <QtGlobal>

DicomController::DicomController(QObject *parent)
    : QObject{parent},
    m_loader(new DicomLoader()),
    m_provider(new DicomImageProvider())
{
    m_imageLoaded = false;
    m_imageVersion = 0;
}

void DicomController::loadFolder(const QString &path)
{
    if (m_loader->loadFolder(path)) {

        m_provider->setImage(
            m_loader->currentImage()
            );

        m_patientName = m_loader->patientName();
        m_studyDate = m_loader->studyDate();

        m_patientId =
            m_loader->patientId();

        m_modality =
            m_loader->modality();

        m_studyDescription =
            m_loader->studyDescription();

        m_seriesDescription =
            m_loader->seriesDescription();

        m_institutionName =
            m_loader->institutionName();

        m_sliceThickness =
            m_loader->sliceThickness();

        m_pixelSpacing =
            m_loader->pixelSpacing();

        m_imageWidth =
            m_loader->imageWidth();

        m_imageHeight =
            m_loader->imageHeight();

        m_imageLoaded = true;

        m_imageVersion++;

        emit patientNameChanged();
        emit studyDateChanged();
        emit patientIdChanged();
        emit modalityChanged();
        emit studyDescriptionChanged();
        emit seriesDescriptionChanged();
        emit institutionNameChanged();
        emit sliceThicknessChanged();
        emit pixelSpacingChanged();
        emit imageSizeChanged();
        emit imageLoadedChanged();
        emit imageVersionChanged();
        emit currentSliceChanged();
        emit totalSlicesChanged();
    }
}

void DicomController::nextSlice()
{
    m_loader->nextSlice();

    m_provider->setImage(
        m_loader->currentImage()
        );

    m_imageVersion++;

    emit imageVersionChanged();
    emit currentSliceChanged();
}

void DicomController::previousSlice()
{
    m_loader->previousSlice();

    m_provider->setImage(
        m_loader->currentImage()
        );

    m_imageVersion++;

    emit imageVersionChanged();
    emit currentSliceChanged();
}

void DicomController::setCurrentSlice(int slice)
{
    if (!m_imageLoaded)
        return;
    const int n = m_loader->totalSlices();
    if (n < 1)
        return;
    const int oneBased = qBound(1, slice, n);
    const int idx = oneBased - 1;
    if (m_loader->currentIndex() == idx)
        return;
    m_loader->goToIndex(idx);
    m_provider->setImage(m_loader->currentImage());
    m_imageVersion++;
    emit imageVersionChanged();
    emit currentSliceChanged();
}


void DicomController::setWindowWidth(double width)
{
    m_loader->setWindowWidth(width);

    m_loader->rerenderAllImages();

    m_provider->setImage(
        m_loader->currentImage()
        );

    m_imageVersion++;

    emit imageVersionChanged();
    emit windowWidthChanged();
}

void DicomController::setWindowLevel(double level)
{
    m_loader->setWindowLevel(level);

    m_loader->rerenderAllImages();

    m_provider->setImage(
        m_loader->currentImage()
        );

    m_imageVersion++;

    emit imageVersionChanged();
    emit windowLevelChanged();
}

double DicomController::windowWidth() const
{
    return m_loader->windowWidth();
}

double DicomController::windowLevel() const
{
    return m_loader->windowLevel();
}

QString DicomController::patientName() const
{
    return m_patientName;
}

QString DicomController::studyDate() const
{
    return m_studyDate;
}

bool DicomController::imageLoaded() const
{
    return m_imageLoaded;
}

int DicomController::imageVersion() const
{
    return m_imageVersion;
}

int DicomController::currentSlice() const
{
    return m_loader->currentIndex() + 1;
}

int DicomController::totalSlices() const
{
    return m_loader->totalSlices();
}

QQmlImageProviderBase* DicomController::provider()
{
    return m_provider;
}

QString DicomController::patientId() const
{
    return m_patientId;
}

QString DicomController::modality() const
{
    return m_modality;
}

QString DicomController::studyDescription() const
{
    return m_studyDescription;
}

QString DicomController::seriesDescription() const
{
    return m_seriesDescription;
}

QString DicomController::institutionName() const
{
    return m_institutionName;
}

QString DicomController::sliceThickness() const
{
    return m_sliceThickness;
}

QString DicomController::pixelSpacing() const
{
    return m_pixelSpacing;
}

int DicomController::imageWidth() const
{
    return m_imageWidth;
}

int DicomController::imageHeight() const
{
    return m_imageHeight;
}
