// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Identify the app so anything using QSettings (window geometry, recent
    // files, ...) stores under a stable key instead of a default one.
    QApplication::setOrganizationName("albertpixels");
    QApplication::setOrganizationDomain("albertpixels.com");
    QApplication::setApplicationName("ALBERT");
    QApplication::setApplicationVersion(ALBERT_VERSION);
    MainWindow window;
    window.show();
    return app.exec();
}
