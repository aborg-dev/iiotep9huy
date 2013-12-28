#pragma once

#include <string>

class Selector
{
public:
  Selector() {}
  virtual ~Selector() {}
};

class FieldSelector : public Selector
{
public:
  FieldSelector(std::string fieldName): fieldName(fieldName) {}
  virtual ~FieldSelector() {}

private:
  std::string fieldName;
};

class ArraySelector : public Selector
{
public:
  ArraySelector(int arrayIndex): arrayIndex(arrayIndex) {}
  virtual ~ArraySelector() {}

private:
  int arrayIndex;
};
