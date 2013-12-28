#pragma once

#include <unordered_map>
#include <stack>

#include "Identifier.hpp"
#include "Value.hpp"
#include "Type.hpp"
#include "Exceptions.hpp"
#include "Selector.hpp"
#include "Procedure.hpp"

class Selector;
class Type;
class Procedure;

class Context
{
public:
	Context();
	~Context();

	void declareVariable(Identifier identifier, Type* varType);

	void declareType(Identifier identifier, Type* type);

	void declareProcedure(Identifier identifier, Procedure* procedure);

	void setVariable(Identifier identifier, Value* value);

	void setVariable(Identifier identifier, Selector* selector, Value* value);

	void declareAndSetVariable(Identifier identifier, Value* value);

	void declareAndReferenceVariable(Identifier identifier, Value* value);

	void removeObsoleteVariables();

	void restoreShadowedVariables();

	Value* getVariableValue(Identifier identifier);
	Value* getVariableValue(Identifier identifier, Selector* selector);

	Value* getVariableReference(Identifier identifier);
	Value* getVariableReference(Identifier identifier, Selector* selector);

	Value* createValueOfType(Identifier typeIdentifier);

	Procedure* getProcedure(Identifier identifier);

	bool variableIsDeclared(Identifier identifier);
	bool procedureIsDeclared(Identifier identifier);
	bool typeIsDeclared(Identifier identifier);

	Type* getType(Identifier identifier);

	struct Variable
	{
		Variable()
		{
			identifier = "";
			value = nullptr;
			declarationLevel = 0;
		}

		Variable(Identifier identifier, Value* value, int declarationLevel):
			identifier(identifier), value(value), declarationLevel(declarationLevel)
		{
		}

		Identifier identifier;
		Value* value;
		int declarationLevel;
	};

	struct ShadowedVariable
	{
		ShadowedVariable()
		{
			variable = nullptr;
			shadowLevel = 0;
		}

		ShadowedVariable(Variable* variable, int shadowLevel):
			variable(variable), shadowLevel(shadowLevel)
		{
		}

		Variable* variable;
		int shadowLevel;
	};

	std::unordered_map<Identifier, Type*> types;
	std::unordered_map<Identifier, Variable*> variables;
	std::unordered_map<Identifier, Procedure*> procedures;

	std::stack<ShadowedVariable*> shadowedVariables;
	std::stack<Variable*> declaredVariables;

	int currentLevel;
};