#!/bin/bash

CXX_FLAGS=

cd build
bison -dv ../test.ypp
flex++ --c++ ../test.l
g++ -c -std=c++0x ../Expression.cpp  -I../ -o expression.o
g++ -c -std=c++0x ../Selector.cpp -I../ -o selector.o
g++ -c -std=c++0x ../Context.cpp -I../ -o context.o
g++ -c -std=c++0x ../Type.cpp -I../ -o type.o
g++ -c -std=c++0x ../Procedure.cpp -I../ -o procedure.o
g++ -c -std=c++0x ../Statement.cpp -I../ -o statement.o
g++ -c -std=c++0x ../Declarations.cpp -I../ -o declarations.o
g++ -c -std=c++0x ../Value.cpp -I../ -o value.o

OBJECTS='expression.o selector.o context.o type.o procedure.o statement.o declarations.o value.o'

g++ -std=c++0x test.tab.cpp lex.yy.cc $OBJECTS -I../ -lm -lfl -o oberon

