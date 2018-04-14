#!/bin/bash

if [[ $# -eq 0 ]]; then
    echo "NO REGEX SPECIFIED!" && exit
fi

source ../../global.sh

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
