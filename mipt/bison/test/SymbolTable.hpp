#pragma once

#include <string>
#include <unordered_map>

#include "VariableType.hpp"

class SymbolTable
{
public:
  SymbolTable();

  // We don't know hashtable policy, so it's not const
  VariableType* getSymbol(std::string symbolName);

  void setSymbol(std::string symbolName, VariableType* value);
  
  void defineSymbol(std::string symbolName);
  bool isDefined(std::string symbolName) const;

private:
  std::unordered_map<std::string, VariableType*> symbolTable;

};
