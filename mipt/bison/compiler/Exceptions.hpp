#pragma once

#include <stdexcept>
#include <string>

class undeclared_variable: public std::exception
{
public:
  undeclared_variable(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};

class undeclared_procedure: public std::exception
{
public:
  undeclared_procedure(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};

class undeclared_type: public std::exception
{
public:
  undeclared_type(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};

class redeclared_variable: public std::exception
{
public:
  redeclared_variable(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};

class changing_const_variable: public std::exception
{
public:
  changing_const_variable(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private: std::string errorMessage;
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

class type_not_found: public std::exception
{
public:
  type_not_found(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};

class redeclared_type: public std::exception
{
public:
  redeclared_type(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};

class redeclared_procedure: public std::exception
{
public:
  redeclared_procedure(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};

class unknown_operator: public std::exception
{
public:
  unknown_operator(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};

class wrong_arguments_number: public std::exception
{
public:
  wrong_arguments_number(const std::string& errorMessage): errorMessage(errorMessage)
  {
  }

  virtual const char* what() const throw()
  {
    return errorMessage.c_str();
  }

private:
  std::string errorMessage;
};
