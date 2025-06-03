#!/usr/local/bin/earl

module Build

set_flag("-x");

let debug = false;
try { debug = ("debug", "d", "deb", "g", "ggdb").contains(argv()[1]); }

if debug {
    $"gcc -DDEBUG -O0 -g -o main *.c -Iinclude/";
} else {
    $"gcc -o main *.c -Iinclude/";
}
