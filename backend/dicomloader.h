#ifndef DICOMLOADER_H
#define DICOMLOADER_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QList>
#include <QStringList>


class DicomLoader : public QObject
{
    Q_OBJECT
public:
    explicit DicomLoader(QObject *parent = nullptr);

    bool loadFile(const QString& filePath);
    bool loadFolder(const QString& folderPath);


    ~DicomLoader();
    QImage  currentImage() const;
    QString patientName()  const;
    QString studyDate()    const;

    QString patientId() const;
    QString modality() const;
    QString studyDescription() const;
    QString seriesDescription() const;
    QString institutionName() const;
    QString sliceThickness() const;
    QString pixelSpacing() const;

    int imageWidth() const;
    int imageHeight() const;

    void nextSlice();
    void previousSlice();
    void goToIndex(int index);
    void setWindowWidth(double width);

    void setWindowLevel(double level);

    // void rerenderCurrentImage();
    void rerenderAllImages();


    int currentIndex() const;
    int totalSlices() const;
    double windowWidth() const;

    double windowLevel() const;



signals:

private:
    QList<QImage> m_images;
    QStringList m_filePaths;

    QImage m_image;

    QString m_patientName;
    QString m_studyDate;

    QString m_patientId = "Unknown";
    QString m_modality = "Unknown";
    QString m_studyDescription = "Unknown";
    QString m_seriesDescription = "Unknown";
    QString m_institutionName = "Unknown";
    QString m_sliceThickness = "Unknown";
    QString m_pixelSpacing = "Unknown";

    int m_imageWidth = 0;
    int m_imageHeight = 0;

    int m_currentIndex = 0;

    double m_windowWidth = 400;
    double m_windowLevel = 40;
};

#endif // DICOMLOADER_H
