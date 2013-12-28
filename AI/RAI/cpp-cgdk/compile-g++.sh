#!/bin/bash

name="MyStrategy"

if [ ! -f ./src/$name.cpp ]
then
    echo Unable to find $name.cpp > compilation.log
    exit 1
fi

rm -f $name

files=""

for i in *.cpp
do
    files="$files $i"
done

for i in ./src/*.cpp
do
    files="$files $i"
done

for i in model/*.cpp
do
    files="$files $i"
done

for i in csimplesocket/*.cpp
do
    files="$files $i"
done

g++ -static -fno-optimize-sibling-calls -fno-strict-aliasing -I. -DONLINE_JUDGE -D_LINUX -lm -s -x c++ -O2 -o $name $files
