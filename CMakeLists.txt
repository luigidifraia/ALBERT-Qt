# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
# The single source of the application version: everything else (the About
# dialog via the ALBERT_VERSION define, QApplication::applicationVersion,
# the macOS bundle metadata) derives from it.
project(albert VERSION 0.2.0 LANGUAGES CXX)

find_package(Qt6
    REQUIRED COMPONENTS Core Gui Widgets
    OPTIONAL_COMPONENTS PrintSupport
)

qt_standard_project_setup()

qt_add_executable(albert
    main.cpp
    mainwindow.cpp mainwindow.h
    canvasarea.cpp canvasarea.h
)

set_target_properties(albert PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# The application version, shown in the About dialog and set on the
# QApplication (main.cpp).
target_compile_definitions(albert PRIVATE ALBERT_VERSION="${PROJECT_VERSION}")

target_link_libraries(albert PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

if (TARGET Qt6::PrintSupport)
    target_link_libraries(albert PRIVATE Qt6::PrintSupport)
endif()

install(TARGETS albert
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET albert
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
