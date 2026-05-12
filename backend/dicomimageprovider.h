#ifndef DICOMIMAGEPROVIDER_H
#define DICOMIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>

class DicomImageProvider : public QQuickImageProvider {
public:
    DicomImageProvider();

    QImage requestImage(const QString& id,
                        QSize* size,
                        const QSize& requestedSize) override;

    void setImage(const QImage& img);

private:
    QImage m_image;
};

#endif // DICOMIMAGEPROVIDER_H
