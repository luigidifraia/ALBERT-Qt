// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "mainwindow.h"
#include "canvasarea.h"

#include <QApplication>
#include <QCloseEvent>
#include <QColorDialog>
#include <QDir>
#include <QFileDialog>
#include <QImageWriter>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#endif

//! [0]
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_canvasArea(new CanvasArea(this))
{
    setCentralWidget(m_canvasArea);

    createActions();
    createMenus();

    setWindowTitle(tr("ALBERT"));
    resize(500, 500); // the default; readSettings() may restore a saved geometry
    readSettings();
}
//! [0]

//! [1]
void MainWindow::closeEvent(QCloseEvent *event)
//! [1] //! [2]
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}
//! [2]

//! [3]
void MainWindow::open()
//! [3] //! [4]
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty() && !m_canvasArea->openImage(fileName)) {
            QMessageBox::warning(this, tr("ALBERT"),
                                 tr("Cannot open file %1.")
                                 .arg(QDir::toNativeSeparators(fileName)));
        }
    }
}
//! [4]

//! [5]
void MainWindow::save()
//! [5] //! [6]
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}
//! [6]

//! [7]
void MainWindow::penColor()
//! [7] //! [8]
{
    QColor newColor = QColorDialog::getColor(m_canvasArea->penColor(), this);
    if (newColor.isValid())
        m_canvasArea->setPenColor(newColor);
}
//! [8]

//! [9]
void MainWindow::penWidth()
//! [9] //! [10]
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("ALBERT"),
                                        tr("Select pen width:"),
                                        m_canvasArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        m_canvasArea->setPenWidth(newWidth);
}
//! [10]

//! [11]
void MainWindow::about()
//! [11] //! [12]
{
    QMessageBox::about(this, tr("About ALBERT"),
            tr("<p><b>ALBERT CE</b> %1 (Advanced Lightweight Bitmap Extender with Raster Tech) "
               "is a pixel art editor for creating extended Commodore 64 images.</p>"
               "<p>Source code: <a href=\"https://github.com/luigidifraia/ALBERT-Qt\">"
               "github.com/luigidifraia/ALBERT-Qt</a></p>")
                .arg(QApplication::applicationVersion()));
}
//! [12]

//! [13]
void MainWindow::createActions()
//! [13] //! [14]
{
    m_openAct = new QAction(tr("&Open..."), this);
    m_openAct->setShortcuts(QKeySequence::Open);
    connect(m_openAct, &QAction::triggered, this, &MainWindow::open);

    const QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();
    for (const QByteArray &format : imageFormats) {
        QString text = tr("%1...").arg(QString::fromLatin1(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, &QAction::triggered, this, &MainWindow::save);
        m_saveAsActs.append(action);
    }

#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    m_printAct = new QAction(tr("&Print..."), this);
    connect(m_printAct, &QAction::triggered, m_canvasArea, &CanvasArea::print);
#endif

    m_exitAct = new QAction(tr("E&xit"), this);
    m_exitAct->setShortcuts(QKeySequence::Quit);
    connect(m_exitAct, &QAction::triggered, this, &MainWindow::close);

    m_penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(m_penColorAct, &QAction::triggered, this, &MainWindow::penColor);

    m_penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(m_penWidthAct, &QAction::triggered, this, &MainWindow::penWidth);

    m_clearScreenAct = new QAction(tr("&Clear Screen"), this);
    m_clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(m_clearScreenAct, &QAction::triggered,
            m_canvasArea, &CanvasArea::clearImage);

    m_aboutAct = new QAction(tr("&About"), this);
    connect(m_aboutAct, &QAction::triggered, this, &MainWindow::about);

    m_aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(m_aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}
//! [14]

//! [15]
void MainWindow::createMenus()
//! [15] //! [16]
{
    QMenu *saveAsMenu = new QMenu(tr("&Save As"), this);
    for (QAction *action : std::as_const(m_saveAsActs))
        saveAsMenu->addAction(action);

    QMenu *fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(m_openAct);
    fileMenu->addMenu(saveAsMenu);
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    fileMenu->addAction(m_printAct);
#endif
    fileMenu->addSeparator();
    fileMenu->addAction(m_exitAct);

    QMenu *optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(m_penColorAct);
    optionMenu->addAction(m_penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(m_clearScreenAct);

    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(m_aboutAct);
    helpMenu->addAction(m_aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}
//! [16]

void MainWindow::readSettings()
{
    QSettings settings;

    const QByteArray geometry = settings.value("window/geometry").toByteArray();
    if (!geometry.isEmpty())
        restoreGeometry(geometry);
}

void MainWindow::writeSettings() const
{
    QSettings settings;

    settings.setValue("window/geometry", saveGeometry());
}

//! [17]
bool MainWindow::maybeSave()
//! [17] //! [18]
{
    if (m_canvasArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("ALBERT"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveFile("png");
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
//! [18]

//! [19]
bool MainWindow::saveFile(const QByteArray &fileFormat)
//! [19] //! [20]
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty())
        return false;
    if (m_canvasArea->saveImage(fileName, fileFormat.constData()))
        return true;

    QMessageBox::warning(this, tr("ALBERT"),
                         tr("Cannot write file %1.")
                         .arg(QDir::toNativeSeparators(fileName)));
    return false;
}
//! [20]
