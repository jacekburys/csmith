#!/bin/bash
../src/csmith --seed ${1} --memcpy --check-global >prog.c
gcc -std=c99 prog.c -w -I../runtime -o prog1
clang -std=c99 prog.c -w -I../runtime -o prog2
./prog1 > out1.txt
./prog2 > out2.txt
