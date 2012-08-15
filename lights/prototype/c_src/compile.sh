#!/bin/sh

#SRC="stalker_pde"
SRC="simpleStalkerIno"

echo "Moving extension .c to .ino..."
cp $SRC.cpp ../$SRC.ino
cd ..
echo "Compiling..."
make
echo
echo "Checking size..."
make size
