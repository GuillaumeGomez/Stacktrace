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


##License

    Copyright (c) 2014 Guillaume Gomez

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
    Come on, don't tell me you read that.
