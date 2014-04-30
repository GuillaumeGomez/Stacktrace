StackTrace
==========

A little code which allows to display stacktrace if your program crash.

To compile on linux (with gcc), please use this command :

gcc -g -rdynamic ./main.c -o test

Under windows, the program needs to be linked to Dbghelp library (add Dbghelp.lib to your dependencies).
