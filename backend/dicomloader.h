#ifndef DICOMLOADER_H
#define DICOMLOADER_H

#include <QObject>
#include <QImage>
#include <QString>

class DicomLoader : public QObject
{
    Q_OBJECT
public:
    explicit DicomLoader(QObject *parent = nullptr);

    bool loadFile(const QString& filePath);

    QImage  currentImage() const;
    QString patientName()  const;
    QString studyDate()    const;

signals:

    private:
    QImage  m_image;
    QString m_patientName;
    QString m_studyDate;

};

#endif // DICOMLOADER_H
