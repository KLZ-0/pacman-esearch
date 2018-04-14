#!/bin/bash

if [[ $# -eq 0 ]]; then
    echo "NO REGEX SPECIFIED!" && exit
fi

CC="g++"

CFLAGS="-Wall -O2 -pipe -march=westmere -std=c++14"
COMPILEDIR="./bin"

CFILE="main.cpp"
OUTFILE="$COMPILEDIR/main.o"

if [ ! -d "$COMPILEDIR" ]; then
    mkdir $COMPILEDIR
fi

if [[ $# -eq 2 ]]; then
    # COMILE
    echo $CC $CFLAGS $CFILE -o ./esearch
    echo "======================================================================="
    echo
    $CC $CFLAGS $CFILE -o ./esearch || exit
fi

# RUN
# time $OUTFILE $1
# echo

# CLEANUP
rm -r $COMPILEDIR
