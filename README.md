# ALBERT-Qt

A from-scratch rewrite of [ALBERT](https://www.albertpixels.com), the pixel art editor for
creating extended Commodore 64 images, using C++ and Qt Widgets instead of C and GTK.

This project is in early scaffolding. See [TODO.md](TODO.md) for the full, ordered feature
checklist of what still needs to be built.

## Origin

The initial project skeleton (`main.cpp`, `mainwindow.h/.cpp`, `canvasarea.h/.cpp`,
`CMakeLists.txt`) started from Qt's official Scribble example, the same way the original GTK
version of ALBERT started from the GTK2 drawing-area demo. The Scribble example is licensed
under BSD-3-Clause; that license header is retained in any file where original example code
still remains.

## Building

Requires Qt 6 and CMake 3.16+.

```sh
git clone https://github.com/luigidifraia/ALBERT-Qt.git
cd ALBERT-Qt
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
windeployqt albert.exe
```

## Contributing

Pick an unchecked item from [TODO.md](TODO.md) whose prerequisites are already done, and keep
pull requests small: one feature (or a tightly related small group) at a time. The published
[user manual](https://www.albertpixels.com/documentation/) and the original GTK project's [change
log](https://www.albertpixels.com/hosted/software/changelog.txt) are the sources of truth for
expected behavior.

## License

MIT - see [LICENSE](LICENSE).
