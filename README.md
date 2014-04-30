StackTrace
==========

A little code which allows to display stacktrace if your program crash.


### On Linux :


To compile on linux (with gcc), please compile with those 2 options :

-g -rdynamic

For example :

gcc -g -rdynamic main.c -o test


### On Windows :

Under windows, the program needs to be linked to Dbghelp library (add Dbghelp.lib to your dependencies).
