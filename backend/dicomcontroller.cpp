#include "dicomcontroller.h"
#include "dicomimageprovider.h"
#include "dicomloader.h"

DicomController::DicomController(QObject *parent)
    : QObject{parent}, m_loader(new DicomLoader()), m_provider(new DicomImageProvider())
{
    // Ensure initial values are set
    emit dataChanged();
}

void DicomController::loadDicom(const QString &path)
{
    if (m_loader->loadFile(path)) {
        m_provider->setImage(m_loader->currentImage());
        m_patientName = m_loader->patientName();
        m_studyDate = m_loader->studyDate();
        m_imageLoaded = true;
        m_imageVersion++;
        emit dataChanged();
    }
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

bool DicomController::imageLoader() const
{
    return m_imageLoaded;
}

QQmlImageProviderBase *DicomController::provider()
{
    return m_provider;
}
