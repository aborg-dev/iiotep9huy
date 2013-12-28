#!/bin/bash

cd src

FILES='*.cpp *.h *.hpp'

zip ../solution.zip $FILES 

echo $FILES
