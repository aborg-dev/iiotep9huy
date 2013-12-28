#include "SymbolTable.hpp"
#include "Exceptions.hpp"

#include <sstream>
#include <iostream>

SymbolTable::SymbolTable()
{
}

VariableType* SymbolTable::getSymbol(std::string symbolName) 
{
  if (!isDefined(symbolName))
  {
    throw undefined_variable("Variable " + symbolName + " is undefined");
  }
  return symbolTable[symbolName];
}

void SymbolTable::setSymbol(std::string symbolName, VariableType* value)
{
  // Not very easy, we need full copy with tree traversing
  //symbolTable[symbolName] = value; 
}

void SymbolTable::defineSymbol(std::string symbolName)
{
  if (isDefined(symbolName))
  {
    throw redefined_variable("Variable " + symbolName + " is already defined");
  }
  std::cerr << "Defining symbol " << symbolName << std::endl;
  // we need to specify type somehow
  //symbolTable[symbolName]
}

bool SymbolTable::isDefined(std::string symbolName) const
{
  return (symbolTable.find(symbolName) != symbolTable.end());
}
