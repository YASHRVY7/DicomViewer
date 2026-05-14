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

        m_imageLoaded = true;

        m_imageVersion++;

        emit patientNameChanged();
        emit studyDateChanged();
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
