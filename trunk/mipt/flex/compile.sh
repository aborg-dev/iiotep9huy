#!/bin/bash

flex task1.flex
g++ -std=c++0x lex.yy.c -lfl -o task1

