// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "canvasarea.h"

#include <QMouseEvent>
#include <QPainter>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include <algorithm>

namespace {

//! [19]
void resizeImage(QImage *image, const QSize &newSize)
//! [19] //! [20]
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
//! [20]

} // namespace

//! [0]
CanvasArea::CanvasArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
}
//! [0]

//! [1]
bool CanvasArea::openImage(const QString &fileName)
//! [1] //! [2]
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    m_image = loadedImage;
    m_modified = false;
    update();
    return true;
}
//! [2]

//! [3]
bool CanvasArea::saveImage(const QString &fileName, const char *fileFormat)
//! [3] //! [4]
{
    QImage visibleImage = m_image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        m_modified = false;
        return true;
    }
    return false;
}
//! [4]

//! [5]
void CanvasArea::setPenColor(const QColor &newColor)
//! [5] //! [6]
{
    m_penColor = newColor;
}
//! [6]

//! [7]
void CanvasArea::setPenWidth(int newWidth)
//! [7] //! [8]
{
    m_penWidth = newWidth;
}
//! [8]

//! [9]
void CanvasArea::clearImage()
//! [9] //! [10]
{
    m_image.fill(qRgb(255, 255, 255));
    m_modified = true;
    update();
}
//! [10]

//! [11]
void CanvasArea::mousePressEvent(QMouseEvent *event)
//! [11] //! [12]
{
    if (event->button() == Qt::LeftButton) {
        m_lastPoint = event->position().toPoint();
        m_drawing = true;
    }
}

void CanvasArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && m_drawing)
        drawLineTo(event->position().toPoint());
}

void CanvasArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_drawing) {
        drawLineTo(event->position().toPoint());
        m_drawing = false;
    }
}

//! [12] //! [13]
void CanvasArea::paintEvent(QPaintEvent *event)
//! [13] //! [14]
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, m_image, dirtyRect);
}
//! [14]

//! [15]
void CanvasArea::resizeEvent(QResizeEvent *event)
//! [15] //! [16]
{
    if (width() > m_image.width() || height() > m_image.height()) {
        int newWidth = (std::max)(width() + 128, m_image.width());
        int newHeight = (std::max)(height() + 128, m_image.height());
        resizeImage(&m_image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}
//! [16]

//! [17]
void CanvasArea::drawLineTo(const QPoint &endPoint)
//! [17] //! [18]
{
    QPainter painter(&m_image);
    painter.setPen(QPen(m_penColor, m_penWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(m_lastPoint, endPoint);
    m_modified = true;

    int rad = (m_penWidth / 2) + 2;
    update(QRect(m_lastPoint, endPoint).normalized()
                                       .adjusted(-rad, -rad, +rad, +rad));
    m_lastPoint = endPoint;
}
//! [18]

//! [21]
void CanvasArea::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
//! [21] //! [22]
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = m_image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(m_image.rect());
        painter.drawImage(0, 0, m_image);
    }
#endif // QT_CONFIG(printdialog)
}
//! [22]
