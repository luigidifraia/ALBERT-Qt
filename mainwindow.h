// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class CanvasArea;

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();

private:
    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);
    // Global application preferences. Read at startup, written on close.
    void readSettings();
    void writeSettings() const;

    CanvasArea *m_canvasArea;

    QAction *m_openAct;
    QList<QAction *> m_saveAsActs;
    QAction *m_exitAct;
    QAction *m_penColorAct;
    QAction *m_penWidthAct;
    QAction *m_printAct = nullptr;
    QAction *m_clearScreenAct;
    QAction *m_aboutAct;
    QAction *m_aboutQtAct;
};
//! [0]

#endif
