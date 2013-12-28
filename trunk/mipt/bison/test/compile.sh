#!/bin/bash

bison -dv test.ypp
flex++ --c++ test.l
g++ -std=c++0x test.tab.cpp lex.yy.cc SymbolTable.cpp VariableType.cpp -lm -lfl -o test

