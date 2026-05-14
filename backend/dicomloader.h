#ifndef DICOMLOADER_H
#define DICOMLOADER_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QList>

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

    void nextSlice();
    void previousSlice();
    void goToIndex(int index);

    int currentIndex() const;
    int totalSlices() const;

signals:

    private:
    QList<QImage> m_images;
    QImage m_image;
    QString m_patientName;
    QString m_studyDate;
    int m_currentIndex = 0;
};

#endif // DICOMLOADER_H
