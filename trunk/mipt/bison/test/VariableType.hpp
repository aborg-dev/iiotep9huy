#pragma once

#include <unordered_map>
#include <string>

class SymbolTable;

class VariableType
{
public:
  virtual bool isString()   const { return false; }
  virtual bool isInteger()  const { return false; }
  virtual bool isBoolean()  const { return false; }
  virtual bool isUserType() const { return false; }
};

template<typename ReturnType>
ReturnType getValue(VariableType* variable) 
{
}

class UserType : public VariableType
{
public:
  virtual bool isUserType() const { return true; }
  SymbolTable* fields;
};

class String : public VariableType
{
public:
  String(std::string value): value(value) {}
  virtual bool isString() const { return true; }
  std::string value;
};

class Integer : public VariableType
{
public:
  Integer(int value): value(value) {}
  virtual bool isInteger()  const { return true; }
  int value;
};

class Boolean : public VariableType
{
public:
  Boolean(bool value): value(value) {}
  virtual bool isBoolean()  const { return true; }
  bool value;
};
