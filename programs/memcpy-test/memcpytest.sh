#!/bin/bash

#CSmith root directory
CSMITH="/home/jacek/Desktop/Imperial/UROP/csmith"

COMP1="gcc"
COMP2="clang"
TIMEOUT="timeout 30 "

RESULTFILE="errors.txt"
TEMPDIR="temp"

makeProg(){
  $TIMEOUT $CSMITH/src/csmith --seed $1 --memcpy > prog.c
  return $?
}

compile(){
  $TIMEOUT $COMP1 -std=c99 -O0 prog.c -w -I${CSMITH}/runtime -o prog-${COMP1}0 &&\
  $TIMEOUT $COMP1 -std=c99 -O1 prog.c -w -I${CSMITH}/runtime -o prog-${COMP1}1 &&\
  $TIMEOUT $COMP1 -std=c99 -O2 prog.c -w -I${CSMITH}/runtime -o prog-${COMP1}2 &&\
  $TIMEOUT $COMP1 -std=c99 -O3 prog.c -w -I${CSMITH}/runtime -o prog-${COMP1}3 &&\
  $TIMEOUT $COMP2 -std=c99 -O0 prog.c -w -I${CSMITH}/runtime -o prog-${COMP2}0 &&\
  $TIMEOUT $COMP2 -std=c99 -O1 prog.c -w -I${CSMITH}/runtime -o prog-${COMP2}1 &&\
  $TIMEOUT $COMP2 -std=c99 -O2 prog.c -w -I${CSMITH}/runtime -o prog-${COMP2}2 &&\
  $TIMEOUT $COMP2 -std=c99 -O3 prog.c -w -I${CSMITH}/runtime -o prog-${COMP2}3
  return $?
}

produceResults(){
  $TIMEOUT ./prog-${COMP1}0 > $TEMPDIR/temp1 &&\
  $TIMEOUT ./prog-${COMP1}1 > $TEMPDIR/temp2 &&\
  $TIMEOUT ./prog-${COMP1}2 > $TEMPDIR/temp3 &&\
  $TIMEOUT ./prog-${COMP1}3 > $TEMPDIR/temp4 &&\
  $TIMEOUT ./prog-${COMP2}0 > $TEMPDIR/temp5 &&\
  $TIMEOUT ./prog-${COMP2}1 > $TEMPDIR/temp6 &&\
  $TIMEOUT ./prog-${COMP2}2 > $TEMPDIR/temp7 &&\
  $TIMEOUT ./prog-${COMP2}3 > $TEMPDIR/temp8
  return $?
}

compareResults(){
  $TIMEOUT diff $TEMPDIR/temp1 $TEMPDIR/temp2 &&\
  $TIMEOUT diff $TEMPDIR/temp2 $TEMPDIR/temp3 &&\
  $TIMEOUT diff $TEMPDIR/temp3 $TEMPDIR/temp4 &&\
  $TIMEOUT diff $TEMPDIR/temp4 $TEMPDIR/temp5 &&\
  $TIMEOUT diff $TEMPDIR/temp5 $TEMPDIR/temp6 &&\
  $TIMEOUT diff $TEMPDIR/temp6 $TEMPDIR/temp7 &&\
  $TIMEOUT diff $TEMPDIR/temp7 $TEMPDIR/temp8
  return $?
}

touch $RESULTFILE
mkdir -p $TEMPDIR

for i in $(seq "$1" "$2"); do
  makeProg $i
  if [ $? -ne 0 ]; then
    echo "timeout $i"
    continue
  fi
  compile
  if [ $? -ne 0 ]; then
    echo "timeout $i"
    continue
  fi
  produceResults
  if [ $? -ne 0 ]; then
    echo "timeout $i"
    continue
  fi
  compareResults
  if [ $? -ne 0 ]; then
    echo "different $i"
    echo "$i" >> "$RESULTFILE"
  else
    echo "same $i"
  fi
done
