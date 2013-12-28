#include "Context.hpp"

Context::Context()
{
	currentLevel = 0;

	/* adding default functions */

	/* read procedure */
	{
		IdentifierList* identifierList = new IdentifierList();
		identifierList->addIdentifier(Identifier("variable"));

		FormalParametersSection* formalParametersSection = 
			new FormalParametersSection(identifierList, nullptr, true);

		FormalParameters* formalParameters = new FormalParameters();
		formalParameters->formalParametersSectionSequence->
			addFormalParametersSection(formalParametersSection);

		ProcedureHeading* procedureHeading = new ProcedureHeading("READ", formalParameters);
		ProcedureBody* procedureBody = new ProcedureBody();
		procedureBody->procedureBlock->addStatement(new ReadStatement("variable"));

		Procedure* readProcedure = new Procedure(procedureHeading, procedureBody);

		declareProcedure(Identifier("READ"), readProcedure);
	}

	/* write procedure */
	{
		IdentifierList* identifierList = new IdentifierList();
		identifierList->addIdentifier(Identifier("variable"));

		FormalParametersSection* formalParametersSection = 
			new FormalParametersSection(identifierList, nullptr, true);

		FormalParameters* formalParameters = new FormalParameters();
		formalParameters->formalParametersSectionSequence->
			addFormalParametersSection(formalParametersSection);

		ProcedureHeading* procedureHeading = new ProcedureHeading("WRITE", formalParameters);
		ProcedureBody* procedureBody = new ProcedureBody();
		procedureBody->procedureBlock->addStatement(new WriteStatement("variable"));

		Procedure* writeProcedure = new Procedure(procedureHeading, procedureBody);
		
		declareProcedure(Identifier("WRITE"), writeProcedure);
	}

	/* adding default functions */
}

Context::~Context()
{
}

void Context::declareVariable(Identifier identifier, Type* varType)
{
	if (!variableIsDeclared(identifier) || variables[identifier]->declarationLevel < currentLevel)
	{
		if (variableIsDeclared(identifier))
		{
			shadowedVariables.push(
				new ShadowedVariable(variables[identifier], currentLevel));
		}
		Variable* variable = 
			new Variable(identifier, varType->createNewValue(this), currentLevel);
		variables[identifier] = variable;
		declaredVariables.push(variable);
	}
	else
	{
		throw redeclared_variable("Variable " + identifier + " is already declared");
	}
}

void Context::declareType(Identifier identifier, Type* type)
{
	if (typeIsDeclared(identifier))
	{
		throw redeclared_type("Type " + identifier + " is already declared");
	}
	types[identifier] = type;
}

void Context::declareProcedure(Identifier identifier, Procedure* procedure)
{
	if (procedureIsDeclared(identifier))
	{
		throw redeclared_procedure("Procedure " + identifier + " is already declared");
	}
	procedures[identifier] = procedure;
}

void Context::setVariable(Identifier identifier, Value* value)
{
	setVariable(identifier, new Selector(), value);
}

void Context::setVariable(Identifier identifier, 
	Selector* selector, Value* value)
{
	if (!variableIsDeclared(identifier))
	{
		throw undeclared_variable("Variable " + identifier + " is undeclared");
	}

	if (variables[identifier]->value->isConst)
	{
		throw changing_const_variable("Trying to change const variable " + identifier);
	}

	selector->select(variables[identifier]->value, this)->setValue(value);
}

void Context::declareAndSetVariable(Identifier identifier, Value* value)
{
	if (!variableIsDeclared(identifier) || variables[identifier]->declarationLevel < currentLevel)
	{
		if (variableIsDeclared(identifier))
		{
			shadowedVariables.push(
				new ShadowedVariable(variables[identifier], currentLevel));
		}
		Variable* variable = 
			new Variable(identifier, value->copy(), currentLevel);
		variables[identifier] = variable;
		declaredVariables.push(variable);
	}
	else
	{
		throw redeclared_variable("Variable " + identifier + " is already declared");
	}
}

void Context::declareAndReferenceVariable(Identifier identifier, Value* value)
{
	if (!variableIsDeclared(identifier) || variables[identifier]->declarationLevel < currentLevel)
	{
		if (variableIsDeclared(identifier))
		{
			shadowedVariables.push(
				new ShadowedVariable(variables[identifier], currentLevel));
		}
		Variable* variable = 
			new Variable(identifier, value, currentLevel);
		variables[identifier] = variable;
		declaredVariables.push(variable);
	}
	else
	{
		throw redeclared_variable("Variable " + identifier + " is already declared");
	}
}

void Context::removeObsoleteVariables()
{
	while (!declaredVariables.empty() && declaredVariables.top()->declarationLevel >= currentLevel)
	{
		Variable* variable = declaredVariables.top();
		declaredVariables.pop();

		Identifier identifier = variable->identifier;

		//std::cerr << "removing obsolete variable " << identifier << std::endl;
		variables.erase(identifier);
	}
}

void Context::restoreShadowedVariables()
{
	//std::cerr << "restoring shadowed variables at level " << currentLevel << std::endl;
	while (!shadowedVariables.empty() && shadowedVariables.top()->shadowLevel == currentLevel)
	{
		ShadowedVariable* shadowedVariable = shadowedVariables.top();
		shadowedVariables.pop();

		Identifier identifier = shadowedVariable->variable->identifier;

		//std::cerr << "restoring shadowed variable " << identifier << std::endl;
		variables[identifier] = shadowedVariable->variable;
	}
}

Value* Context::getVariableValue(Identifier identifier)
{
	return getVariableValue(identifier, new Selector());
}

Value* Context::getVariableValue(Identifier identifier, Selector* selector)
{
	if (!variableIsDeclared(identifier))
	{
		throw undeclared_variable("Variable " + identifier + " is undeclared");
	}

	return selector->select(variables[identifier]->value, this)->copy();
}

Value* Context::getVariableReference(Identifier identifier)
{
	return getVariableReference(identifier, new Selector());
}

Value* Context::getVariableReference(Identifier identifier, Selector* selector)
{
	if (!variableIsDeclared(identifier))
	{
		throw undeclared_variable("Variable " + identifier + " is undeclared");
	}

	return selector->select(variables[identifier]->value, this);
}

Value* Context::createValueOfType(Identifier typeIdentifier)
{
	return getType(typeIdentifier)->createNewValue(this);
}

Procedure* Context::getProcedure(Identifier identifier)
{
	if (!procedureIsDeclared(identifier))
	{
		throw undeclared_procedure("Procedure " + identifier + " is undeclared");
	}

	return procedures[identifier];
}

bool Context::variableIsDeclared(Identifier identifier)
{
	return (variables.find(identifier) != variables.end());
}

bool Context::procedureIsDeclared(Identifier identifier)
{
	return (procedures.find(identifier) != procedures.end());
}

bool Context::typeIsDeclared(Identifier identifier)
{
	return (types.find(identifier) != types.end());
}

Type* Context::getType(Identifier identifier)
{
	if (!typeIsDeclared(identifier))
	{
		throw undeclared_type("Type " + identifier + " is undeclared");
	}
	return types[identifier];	
}
