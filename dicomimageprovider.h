#ifndef DICOMIMAGEPROVIDER_H
#define DICOMIMAGEPROVIDER_H

#include <QObject>

class DicomImageProvider : public QObject
{
    Q_OBJECT
public:
    explicit DicomImageProvider(QObject *parent = nullptr);

signals:
};

#endif // DICOMIMAGEPROVIDER_H
