#pragma once

#include <string>
#include <iostream>

#include "Declarations.hpp"
#include "Identifier.hpp"
#include "Context.hpp"
#include "Statement.hpp"

class Module
{
public:
  Module(Identifier moduleName, 
    Declarations declarations, 
    StatementSequence mainBlock = StatementSequence()): 
    moduleName(moduleName),
    declarations(declarations),
    mainBlock(mainBlock) {}

  void declare()
  {
  	//std::cerr << "Running declaration in module " << moduleName << std::endl;
    moduleContext.declareType("INTEGER", new IntegerType());
    moduleContext.declareType("DOUBLE", new DoubleType());
    moduleContext.declareType("BOOLEAN", new BooleanType());
    moduleContext.declareType("STRING", new StringType());
  	declarations.declare(moduleContext);
  }

  void runMain()
  {
  	//std::cerr << "Running main in module " << moduleName << std::endl;
		mainBlock.execute(moduleContext);
  }

  void runProcedure(Identifier procedureName, ActualParameters* actualParameters)
  {
  	//std::cerr << "Running procedure " << procedureName << " in module " << moduleName << std::endl;
    moduleContext.getProcedure(procedureName)->call(actualParameters, moduleContext);
  }

private:
	Identifier moduleName;
  Declarations declarations;
  StatementSequence mainBlock;
  Context moduleContext;
};
