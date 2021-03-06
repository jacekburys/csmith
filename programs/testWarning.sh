#!/bin/bash

ADAPTER_PATH='/home/jacek/Desktop/Imperial/UROP/Adapter/boost_interval_adapter/adapter.o'
RUNTIME_PATH='/home/jacek/Desktop/Imperial/UROP/csmith/runtime'
CSMITH_PATH='/home/jacek/Desktop/Imperial/UROP/csmith/src/csmith'
LOG_FILE='warningsLog.txt'

touch $LOG_FILE

TIMEOUT=0
ERROR=0
OK=0

for i in $(seq ${1} ${2}); do
  timeout 60 ${CSMITH_PATH} --seed $i --strict-float --float-test --check-global >prog.c
  if [ $? -ne 0 ]; then
    TIMEOUT=$((TIMEOUT+1))
    echo "$i TIMEOUT $OK  $ERROR  $TIMEOUT"
    continue
  fi
  timeout 30 gcc prog.c -I${RUNTIME_PATH} -c -o prog.o > err.txt 2>&1
  timeout 30 g++ prog.o ${ADAPTER_PATH} -o prog
  if grep 'incompatible' err.txt; then
    ERROR=$((ERROR+1))
    echo "$i ERROR $OK  $ERROR  $TIMEOUT"
  else
    OK=$((OK+1))
    echo "$i OK $OK  $ERROR  $TIMEOUT"
  fi
  echo "$i : OK $OK, ERROR $ERROR, TIMEOUT $TIMEOUT" > "$LOG_FILE"
done
