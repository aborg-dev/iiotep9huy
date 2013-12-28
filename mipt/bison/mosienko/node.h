#pragma once

#include <stdio.h>

class TNode {
public:
    virtual void Print() = 0;
};

class TNumber : public TNode {
public:
    TNumber(int V) : Value(V) { }
    
    virtual void Print() {
        printf("%d", Value);
    }
    
    int Value;
};

class TVariable : public TNode {
public:
    TVariable(char N) : Name(N) { }
    
    virtual void Print() {
        printf("%c", Name);
    }
    
    char Name;
};

class TOperation : public TNode {
public:
    TOperation(char O) : Operation(O), Left(NULL), Right(NULL) { }
    
    virtual void Print() {
        printf("(");
        Left->Print();
        printf(" %c ", Operation);
        Right->Print();
        printf(")");
    }
    
    char Operation;
    TNode * Left;
    TNode * Right;
};
