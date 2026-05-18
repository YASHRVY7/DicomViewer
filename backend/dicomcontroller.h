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

    Q_PROPERTY(double windowWidth
                   READ windowWidth
                       NOTIFY windowWidthChanged)

    Q_PROPERTY(double windowLevel
                   READ windowLevel
                       NOTIFY windowLevelChanged)

    Q_PROPERTY(QString patientId
                   READ patientId
                       NOTIFY patientIdChanged)

    Q_PROPERTY(QString modality
                   READ modality
                       NOTIFY modalityChanged)

    Q_PROPERTY(QString studyDescription
                   READ studyDescription
                       NOTIFY studyDescriptionChanged)

    Q_PROPERTY(QString seriesDescription
                   READ seriesDescription
                       NOTIFY seriesDescriptionChanged)

    Q_PROPERTY(QString institutionName
                   READ institutionName
                       NOTIFY institutionNameChanged)

    Q_PROPERTY(QString sliceThickness
                   READ sliceThickness
                       NOTIFY sliceThicknessChanged)

    Q_PROPERTY(QString pixelSpacing
                   READ pixelSpacing
                       NOTIFY pixelSpacingChanged)

    Q_PROPERTY(int imageWidth
                   READ imageWidth
                       NOTIFY imageSizeChanged)

    Q_PROPERTY(int imageHeight
                   READ imageHeight
                       NOTIFY imageSizeChanged)
public:
    explicit DicomController(QObject *parent = nullptr);

    Q_INVOKABLE void loadFolder(const QString& path);

    Q_INVOKABLE void nextSlice();
    Q_INVOKABLE void previousSlice();
    Q_INVOKABLE void setCurrentSlice(int slice);
    Q_INVOKABLE void setWindowWidth(double width);
    Q_INVOKABLE void setWindowLevel(double level);


    QString patientName() const;
    QString studyDate() const;

    QString patientId() const;
    QString modality() const;
    QString studyDescription() const;
    QString seriesDescription() const;
    QString institutionName() const;
    QString sliceThickness() const;
    QString pixelSpacing() const;

    int imageWidth() const;
    int imageHeight() const;

    bool imageLoaded() const;

    int imageVersion() const;

    int currentSlice() const;
    int totalSlices() const;

    double windowWidth() const;

    double windowLevel() const;

    QQmlImageProviderBase* provider();


signals:
    void patientNameChanged();
    void studyDateChanged();
    void imageLoadedChanged();
    void imageVersionChanged();
    void currentSliceChanged();
    void totalSlicesChanged();
    void windowWidthChanged();
    void windowLevelChanged();
    void patientIdChanged();
    void modalityChanged();
    void studyDescriptionChanged();
    void seriesDescriptionChanged();
    void institutionNameChanged();
    void sliceThicknessChanged();
    void pixelSpacingChanged();
    void imageSizeChanged();
private:

    DicomLoader* m_loader;
    DicomImageProvider* m_provider;

    QString m_patientName = "Unknown";
    QString m_studyDate = "Unknown";

    QString m_patientId = "Unknown";
    QString m_modality = "Unknown";
    QString m_studyDescription = "Unknown";
    QString m_seriesDescription = "Unknown";
    QString m_institutionName = "Unknown";
    QString m_sliceThickness = "Unknown";
    QString m_pixelSpacing = "Unknown";

    int m_imageWidth = 0;
    int m_imageHeight = 0;

    int m_imageVersion = 0;
    bool m_imageLoaded = false;

};

#endif // DICOMCONTROLLER_H
