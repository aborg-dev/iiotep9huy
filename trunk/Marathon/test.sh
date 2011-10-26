#!/bin/bash

if [ -e res.txt ]; then
  rm res.txt
fi

for((i=10; i<10000; i+=1000))
{
  echo Calculating $i
  java -jar Vis.jar -dot -v "$i" -n $i -seed 1 -x "./solution"  >>  res.txt
}

