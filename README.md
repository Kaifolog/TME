# <h1 align="center">TME - Turing machine emulator</h1>

<p align="center">
<img src="https://img.shields.io/badge/C%2B%2B-11-ff69b4">
<img src="https://img.shields.io/badge/License-MIT-brightgreen">
</p>

<p align="center">
<img src="./assets/logo.jpg" width="30%">
</p>

## Motivation
TME is designed to work efficiently with a Turing machine.
There are several similar emulators. What are their pros and cons?

| Features       | TME                | Dickarev | Onishenko |
| -------------- |:------------------:| --------:| ---------:|
| Comments       | +                  |     +    |     -     |
| Breakpoints    | +                  |     +    |     -     |
| Macros         | +                  |     -    |     -     |
| Dark theme     | +                  |     -    |     -     |
| Stable         | +                  |     +    |     -     |
| Open source    | +                  |     -    |     -     |
| Software maintenance  | +                  |     -    |     -     |
| UNIX version   | +                  |     -    |     -     |
| console version   | +                  |     -    |     -     |
| UNIX console version   | +                  |     -    |     -     |

As you see,a large number of drawbacks of its counterparts is fixed.

## **First of all**
You **have to** read the <a href="https://github.com/Kaifolog/TME/raw/master/instruction.pdf">instruction</a>. It describes emulator command language, building and many other important aspects.



## Example
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

## Requirements
#### **Windows:**
MinGW toolchain (gcc, g++, mingw32-make) with setted environmental variables, cmake, and QT in dynamic linking distribution (optional, only when gui is necessary).

#### **Linux and Mac:**
GNU gcc, g++, make; cmake, and QT in dynamic linking distribution (optional, only for gui version).

## Build

You probably want to keep your file system neet. So, you should make a directory ```build``` to separate source and temporary files.

Moreover, you can choose between console and gui version by -DIS_GUI=(True or False) flag. If you choose gui version, so, you have to set qt binary path by flag -DCMAKE_PREFIX_PATH.

#### **Windows:**
```
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="D:/Qt/5.15.2/mingw81_64" -DIS_GUI=True
mingw32-make
```

#### **Linux:**
```
sudo apt install qt5-default
mkdir build && cd build && cmake .. && make
```
#### **MacOS:**
```
Coming soon ;(
```