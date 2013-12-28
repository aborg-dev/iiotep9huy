#include "VariableType.hpp"

template<>
std::string getValue(VariableType* variable)
{
  return dynamic_cast<String*>(variable)->value;
}

template<>
int getValue(VariableType* variable)
{
  return dynamic_cast<Integer*>(variable)->value;
}

template<>
bool getValue(VariableType* variable)
{
  return dynamic_cast<Boolean*>(variable)->value;
}
