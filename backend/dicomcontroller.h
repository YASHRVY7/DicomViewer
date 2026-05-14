#ifndef DICOMCONTROLLER_H
#define DICOMCONTROLLER_H

#include <QObject>
#include <QQuickImageProvider>

class DicomImageProvider;
class DicomLoader;

class DicomController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString patientName
                   READ patientName
                       NOTIFY patientNameChanged)

    Q_PROPERTY(QString studyDate
                   READ studyDate
                       NOTIFY studyDateChanged)

    Q_PROPERTY(bool imageLoaded
                   READ imageLoaded
                       NOTIFY imageLoadedChanged)

    Q_PROPERTY(int imageVersion
                   READ imageVersion
                       NOTIFY imageVersionChanged)

    Q_PROPERTY(int currentSlice
                   READ currentSlice
                       NOTIFY currentSliceChanged)

    Q_PROPERTY(int totalSlices
                   READ totalSlices
                       NOTIFY totalSlicesChanged)
public:
    explicit DicomController(QObject *parent = nullptr);

    Q_INVOKABLE void loadFolder(const QString& path);

    Q_INVOKABLE void nextSlice();
    Q_INVOKABLE void previousSlice();
    Q_INVOKABLE void setCurrentSlice(int slice);

    QString patientName() const;
    QString studyDate() const;

    bool imageLoaded() const;

    int imageVersion() const;

    int currentSlice() const;
    int totalSlices() const;

    QQmlImageProviderBase* provider();


signals:
    void patientNameChanged();
    void studyDateChanged();
    void imageLoadedChanged();
    void imageVersionChanged();
    void currentSliceChanged();
    void totalSlicesChanged();
private:

    DicomLoader* m_loader;
    DicomImageProvider* m_provider;

    QString m_patientName = "Unknown";
    QString m_studyDate = "Unknown";

    int m_imageVersion = 0;
    bool m_imageLoaded = false;

};

#endif // DICOMCONTROLLER_H
