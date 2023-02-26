# <h1 align="center">TME - Turing machine emulator</h1>

<p align="center">
<img src="https://img.shields.io/badge/C%2B%2B-11-ff69b4">
<img src="https://img.shields.io/badge/License-MIT-brightgreen">
</p>

<p align="center">
<img src="./assets/icon.png" width="20%">
</p>

# Motivation
TME is designed to work efficiently with a Turing machine.
There are several similar emulators. What are their pros and cons?

| Features       | TME                | Dickarev | Onishenko |
| -------------- |:------------------:| --------:| ---------:|
| Comments       | +                  |     +    |     -     |
| Breakpoints    | +                  |     +    |     -     |
| Macros         | +                  |     -    |     -     |
| Themes         | +                  |     -    |     -     |
| Stable         | +                  |     +    |     -     |
| Open source    | +                  |     -    |     -     |
| Software maintenance  | +           |     -    |     -     |
| UNIX version          | +           |     -    |     -     |
| console version       | +           |     -    |     -     |
| UNIX console version  | +           |     -    |     -     |
| Hello kitty friendly  | +           |     -    |     -     |

As you see,a large number of drawbacks of its counterparts is fixed.

# Demo
<p align="center">
<img src="https://kaifolog.github.io/TME-website/assets/tme-demo-2-0-0b.gif" width="80%">
</p>

# Example
[ [`examples/HelloWorld.txt`](examples/HelloWorld.txt) ]
```
section .data
|lambda|
section .text
start, lambda    	 ->  world, Hello,r	;#d
world, lambda -> name, World,r
name, lambda -> exclamation, %username%, r
exclamation, lambda -> end, !, r
```
<summary>Output</summary>

```
Hello World %username% !
```

- - -

# **First of all**
You **have to** read the <a href="https://github.com/Kaifolog/TME/blob/master/assets/LaTeX/instruction.pdf">instruction</a>. It describes emulator command language, building and many other important aspects.

# Distribution

The program has several distribution options:
1. Portable or not, statically compiled binary that you can get in the Releases section of this page or on the [product site](https://kaifolog.github.io/TME-website/).
2. You can build your own version of the program by following the instructions(for static linking) in [CONTRIBUTING.md](CONTRIBUTING.md) file or (for dynamic linking) below:

## Build & Launch
You probably want to keep your file system neet. So, you should make a directory ```build``` to separate source and temporary files.

Moreover, you can choose between console and GUI version by -D IS_GUI=(True or False) flag. If you choose GUI version, you should set qt binary path by flag -D CMAKE_PREFIX_PATH.

### **Windows:**
#### **Requirements**
- MinGW toolchain
- Cmake
- Qt5
- [OpenSSL 1.1](https://slproweb.com/products/Win32OpenSSL.html)

#### **Building**
```
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="D:/utils/Qt/5.15.2/mingw81_64" -DGUI=True -Wno-dev
mingw32-make
```

### **Linux:**
#### **Requirements**
- GNU gcc, g++, make
- Cmake
- Qt5
- openssl openssl-dev
#### **Qt5 Installation**
```
sudo apt install qt5-default
```
#### **Building**
```
mkdir build && cd build && cmake .. -DGUI=True && make
```
### **MacOS:**
To begin with, you need to install brew on your mac.
```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
#### **Requirements**
- Clang
- Cmake
- Qt5
- OpenSSL
#### **Qt5 Installation**
```
brew install qt@5
```
The next step we need to do is to find Qt5 folder.
```
-> brew --cellar qt@5
   /opt/homebrew/Cellar/qt@5
-> cd /opt/homebrew/Cellar/qt@5
-> ls
   5.15.2_1
-> cd 5.15.2_1
-> pwd
   /opt/homebrew/Cellar/qt@5/5.15.2_1
```
**USE YOUR OWN QT5 PATH AND VERSION NUMER**

Copy this path.
#### **Building**
Let's start building:
```
mkdir -p build && cd build && cmake .. -DIS_GUI=True -DCMAKE_PREFIX_PATH="$" && make
```
**INSTEAD OF $ SIGN YOU NEED TO PASTE THE QT5 PATH**

Now you can run the program:
```
cd src && ./TME
```
