# <h1 align="center">TME - Turing machine emulator</h1>

<p align="center">
<img src="https://img.shields.io/badge/C%2B%2B-11-ff69b4">
<img src="https://img.shields.io/badge/License-MIT-brightgreen">
</p>

<p align="center">
<center><img src="./logo.jpg" width="30%"></center>
</p>
<div style="text-align:center"><img src="./logo.jpg" width="30%" /></div>


## **First of all**
You **have to** read <a href="https://github.com/Kaifolog/TME/raw/master/instruction.pdf">instruction</a>. Which describes emulator command language, building and many other important aspects.


## Motivation

There are several similar emulators. What are their pros and cons?

| Features       | TME                | Dickarev | Onishenko |
| -------------- |:------------------:| --------:| ---------:|
| Comments       | +                  |     +    |     -     |
| UNIX version   | +                  |     -    |     -     |
| Breakpoints    | +                  |     +    |     -     |
| Macros         | +                  |     -    |     -     |
| Dark theme     | +                  |     -    |     -     |
| Stable         | +                  |     +    |     -     |
| Open source    | +                  |     -    |     -     |
| Software maintenance  | +                  |     -    |     -     |


## Example
[ [`tests/HelloWorld.txt`](tests/HelloWorld.txt) ]
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

## Build
>To build it you have to use CMake with g++.<p>
You have to execute commands from __\$project folder\$/bin__ folder.



#### **Windows:**
```
cmake ../ && cmake --build ./ && start ./Debug/TME.exe
```

#### **UNIX-like:**
```
cmake ../ && cmake --build ./ && ./TME
```
