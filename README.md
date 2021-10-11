# <center>TME - Turing machine emulator.</center>

<center><svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width="92" height="20" role="img" aria-label="version: 1.002"><title>version: 1.002</title><linearGradient id="s" x2="0" y2="100%"><stop offset="0" stop-color="#bbb" stop-opacity=".1"/><stop offset="1" stop-opacity=".1"/></linearGradient><clipPath id="r"><rect width="92" height="20" rx="3" fill="#fff"/></clipPath><g clip-path="url(#r)"><rect width="51" height="20" fill="#555"/><rect x="51" width="41" height="20" fill="#dfb317"/><rect width="92" height="20" fill="url(#s)"/></g><g fill="#fff" text-anchor="middle" font-family="Verdana,Geneva,DejaVu Sans,sans-serif" text-rendering="geometricPrecision" font-size="110"><text aria-hidden="true" x="265" y="150" fill="#010101" fill-opacity=".3" transform="scale(.1)" textLength="410">version</text><text x="265" y="140" transform="scale(.1)" fill="#fff" textLength="410">version</text><text aria-hidden="true" x="705" y="150" fill="#010101" fill-opacity=".3" transform="scale(.1)" textLength="310">1.002</text><text x="705" y="140" transform="scale(.1)" fill="#fff" textLength="310">1.002</text></g></svg> 
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width="82" height="20" role="img" aria-label="License: MIT"><title>License: MIT</title><linearGradient id="s" x2="0" y2="100%"><stop offset="0" stop-color="#bbb" stop-opacity=".1"/><stop offset="1" stop-opacity=".1"/></linearGradient><clipPath id="r"><rect width="82" height="20" rx="3" fill="#fff"/></clipPath><g clip-path="url(#r)"><rect width="51" height="20" fill="#555"/><rect x="51" width="31" height="20" fill="#4c1"/><rect width="82" height="20" fill="url(#s)"/></g><g fill="#fff" text-anchor="middle" font-family="Verdana,Geneva,DejaVu Sans,sans-serif" text-rendering="geometricPrecision" font-size="110"><text aria-hidden="true" x="265" y="150" fill="#010101" fill-opacity=".3" transform="scale(.1)" textLength="410">License</text><text x="265" y="140" transform="scale(.1)" fill="#fff" textLength="410">License</text><text aria-hidden="true" x="655" y="150" fill="#010101" fill-opacity=".3" transform="scale(.1)" textLength="210">MIT</text><text x="655" y="140" transform="scale(.1)" fill="#fff" textLength="210">MIT</text></g></svg>
</center>

<center><img src="./logo.jpg" width="30%"></center>


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
