#ifndef DICOMCONTROLLER_H
#define DICOMCONTROLLER_H

#include <QObject>
#include <QQuickImageProvider>

class DicomImageProvider;
class DicomLoader;

class DicomController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString patientName READ patientName NOTIFY dataChanged)
    Q_PROPERTY(QString studyDate   READ studyDate   NOTIFY dataChanged)
    Q_PROPERTY(bool imageLoaded READ imageLoaded NOTIFY dataChanged)
    Q_PROPERTY(int imageVersion READ imageVersion NOTIFY dataChanged)
    Q_PROPERTY(bool imageLoader READ imageLoader NOTIFY dataChanged)

public:
    explicit DicomController(QObject *parent = nullptr);

    Q_INVOKABLE void loadDicom(const QString& path);

    QString patientName() const;
    QString studyDate() const;
    bool imageLoaded() const;
    int imageVersion() const;
    bool imageLoader() const;

    QQmlImageProviderBase* provider();


signals:
    void dataChanged();
private:

    DicomLoader* m_loader;
    DicomImageProvider* m_provider;

    QString m_patientName = "Unknown";
    QString m_studyDate = "Unknown";

    int m_imageVersion = 0;
    bool m_imageLoaded = false;

};

#endif // DICOMCONTROLLER_H
