#include "dicomimageprovider.h"

DicomImageProvider::DicomImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image) {}

QImage DicomImageProvider::requestImage(const QString&,
                                        QSize* size,
                                        const QSize&) {
    if (m_image.isNull()) {
        QImage empty(1,1,QImage::Format_Grayscale8);
        empty.fill(Qt::black);
        if (size) *size = empty.size();
        return empty;
    }

    if (size) *size = m_image.size();
    return m_image;
}

void DicomImageProvider::setImage(const QImage& img) {
    m_image = img;
}
