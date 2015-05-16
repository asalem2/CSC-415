# Homework 1 - Hello World

For this assignment you will write a simple program in C that prints out the following string on the
shell command line: Hello 415, I am <your name>! where <your name> is replaced by
your name (you should store the value of your name in a #DEFINE'd constant).

You will implement two versions of your program: one executing on a Linux platform and
implemented using POSIX system calls, and the other executing on a Windows platform and
implemented using Win32 system calls. At least one of the platforms needs to be a virtual machine. The
Linux version should be implemented using a single low level write() (using the integer 1 as the file
descriptor to represent standard out), along with the sprintf() utility to format your output into a
buffer prior to printing. Check the Linux man pages for details on how to use POSIX system calls
(including the #include directives you will need for each one). Use gcc to compile your program.
The Win32 version of your program should call GetStdHandle() to get a handle to the standard
output stream, followed by a call to the low level function WriteFile(). Again, you should use the
sprintf() function to format your output into a buffer prior to printing. Check the MSDN
documentation for details on how to use Win32 system calls. Use cl to compile your program, making
sure to adhere to the C90 standard (which means among other things that variable declarations must all
be at the beginning of blocks).

You should submit your source code files (one for each platform) and a short writeup in pdf format that
includes a description of what you did, the compilation and execution output from each of your
programs, and a screen capture of your virtual machine running inside your host operating system.
Submit everything (including the writeup) to the regular submission link on iLearn, and then submit
just the writeup to the TurnItIn link to generate an originality report.
