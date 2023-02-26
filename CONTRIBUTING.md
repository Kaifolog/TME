# Welcome to TME contributing guide <!-- omit in toc -->

Thank you for investing your time in contributing to my project! :sparkles:

## Getting started

To begin with you should read the instruction and documentation.

## Commit your changes

Before committing any update you should verify your changes by linters and formatters.

The project uses clang toolchain therefore you should use clang-format and clang-tidy tools.

### clang-format

> You should **NEVER** format themes or text modules.

The project uses Microsoft code style:

```
clang-format -style=Microsoft -dump-config > .clang-format
```

**Before committing** you should apply formatter by command:

```
find ./src/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i
```

The last step before committing is to try to build the current application version.

Project primarly (except names of files and include guards) uses the google c++ code style :

```
user@machine:~/.../TME$ clang-tidy ./src/gui/mainwindow.cpp
```

## Releasing

### Windows

Firstly you should set up development enviroment.

#### OpenSSL
You should compile OpenSSL statically.

#### Qt
You should compile Qt statically. Current project uses Qt 5.15.2 but you can use any version if you are ready for uncertain consequences.

Download qt: https://download.qt.io/archive/qt/5.15/5.15.2/single/
Then follow that guide: https://habr.com/ru/post/562072/
Use this configuration: 
```
configure.bat -static -static-runtime -release -platform win32-g++ -prefix %QT_INSTALL_PREFIX% -qt-zlib -qt-pcre -qt-libpng -qt-libjpeg -qt-freetype -opengl desktop -opensource -confirm-license -make libs -nomake tools -nomake examples -nomake tests -qt-tiff -qt-webp -no-pch -optimize-size -openssl-linked -I "D:\utils\openssl_x64_static\include" -L "D:\utils\openssl_x64_static\lib" OPENSSL_LIBS="-LD:\utils\openssl_x64_static\lib -llibssl -llibcrypto -luser32 -lgdi32 -lwsock32 -lws2_32"
```

#### Resources
Follow that guide: https://stackoverflow.com/questions/708238/how-do-i-add-an-icon-to-a-mingw-gcc-compiled-executable

#### Static building

Use GUI and TME_GUI_WIN_STATIC CMake options while compiling CMake. 

#### Verification

Then you should verify your build with Microsoft. (https://stackoverflow.com/questions/48946680/how-to-avoid-the-windows-defender-smartscreen-prevented-an-unrecognized-app-fro/66582477#66582477)

```That's it!```