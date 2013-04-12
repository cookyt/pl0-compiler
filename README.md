PL/0 Compiler/VM
================
Authors
-------
+ Carlos Valera
+ Jon Hellmann

Description
-----------
This is a compiler for PL/0. This was written for a university course on
introductory compiler design. It compiles to a pseudo assembly language called
`p-code`. The executable also includes an interpreter for p-code.

As input, the program takes a PL/0 program. It compiles it into p-code, then
executes it. There are several flags for runtime stack-tracing and showing
scanner/parser intermmediate output. Use `./plrun -h` to see what they are.


Compilation
-----------
1. Run `make`. This will create the `plrun` executable in the root directory

2. To run the PL/0 Compiler/VM, type the following, substituting the names
   for the input and output files.

    ./plrun -i input_file -o output_file
    

Notes
-----
Some example programs are available in the `test/` directory.

The program has only been tested on 64 bit Ubuntu 12.10 Linux. It only uses
standard libraries, so it should compile on just about anything.
