#pragma once

#include <stdexcept>
#include <string>

class undefined_variable: public std::exception
{
public:
  undefined_variable(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};

class redefined_variable: public std::exception
{
public:
  redefined_variable(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};

class wrong_type: public std::exception
{
public:
  wrong_type(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};
