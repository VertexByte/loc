Lines Of Code - LOC

- Version 1.0
  -> 764 lines of code
  -> managed to load a ~1.2 mil project in ~2.3 sec
  
Programming languages suported:
- C
- C++
- C/C++ Header
- C#
- Css
- Java
- JavaScript
- Python
- PHP
- Go
- HTML
- Batch

---- Usage ----
Parameters: files or directories
   Example:
         >loc source.cpp              -Single file
	 
         >loc c:/work or c:/work/     -Directory

         >loc source.cpp main.cpp c:/work/project c:/work/project2/ - Combined


TODO
 - Empty for now.

Done so far:
- Switch to Win32 File IO
- Differentiate between files and directories.
- Make directories a valid input.
- Implement an easy way to add new file types as valid inputs.
- Add a simple way to mesure performance of the program.
- Have one big buffer for a file's memory rather than allocation memory
  every time we want to read a file.(Its a 64 mb buffer)
- Show lines of code per programming language in project.
- Optimized the line processing routine by 25%


Licnese:
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
