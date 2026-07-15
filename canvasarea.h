// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef CANVASAREA_H
#define CANVASAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

//! [0]
class CanvasArea : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasArea(QWidget *parent = nullptr);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    bool isModified() const { return m_modified; }
    QColor penColor() const { return m_penColor; }
    int penWidth() const { return m_penWidth; }

public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);

    bool m_modified = false;
    bool m_drawing = false;
    int m_penWidth = 1;
    QColor m_penColor = Qt::blue;
    QImage m_image;
    QPoint m_lastPoint;
};
//! [0]

#endif
