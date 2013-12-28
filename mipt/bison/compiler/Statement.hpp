#pragma once

#include <vector>
#include <iostream>

#include "Context.hpp"
#include "Identifier.hpp"
#include "Expression.hpp"
#include "Exceptions.hpp"
#include "Procedure.hpp"

class ActualParameters;
class Selector;

class Statement
{
public:
	Statement() {}
	virtual ~Statement() {}

	virtual void execute(Context& context);
};

class StatementSequence
{
public:
	StatementSequence() {}
	~StatementSequence() {}

	void execute(Context& context);

	void addStatement(Statement* statement);

	std::vector<Statement*> statements;
};

class AssignmentStatement : public Statement
{
public:
	AssignmentStatement(Identifier identifier, Selector* selector, Expression* expression):
		identifier(identifier), selector(selector), expression(expression) {}
	virtual ~AssignmentStatement() {}

	virtual void execute(Context& context);

	Identifier identifier;
	Selector* selector;
	Expression* expression;
};

class IfStatement : public Statement
{
public:
	IfStatement(Expression* expression, 
							StatementSequence* ifStatementSequence,
							StatementSequence* elseStatementSequence = new StatementSequence()): 
		expression(expression), 
		ifStatementSequence(ifStatementSequence),
		elseStatementSequence(elseStatementSequence) {}

	~IfStatement() {}

	virtual void execute(Context& context);

	Expression* expression;	
	StatementSequence* ifStatementSequence;
	StatementSequence* elseStatementSequence;
};

class WhileStatement : public Statement
{
public:
	WhileStatement(Expression* expression, StatementSequence* statementSequence):
		expression(expression), statementSequence(statementSequence) {}

	~WhileStatement() {}

	virtual void execute(Context& context);

	Expression* expression;	
	StatementSequence* statementSequence;
};

class ProcedureCallStatement : public Statement
{
public:
	ProcedureCallStatement(Identifier identifier, 
													ActualParameters* actualParameters):
		identifier(identifier), 
		actualParameters(actualParameters) {}

	~ProcedureCallStatement() {}

	virtual void execute(Context& context);

	Identifier identifier;
	ActualParameters* actualParameters;
};

class ReadStatement : public Statement
{
public:
		ReadStatement(Identifier identifier):
			identifier(identifier)
		{
		}

		~ReadStatement() {}

		virtual void execute(Context& context);

		Identifier identifier;
};

class WriteStatement : public Statement
{
public:
		WriteStatement(Identifier identifier):
			identifier(identifier)
		{
		}

		~WriteStatement() {}

		virtual void execute(Context& context);

		Identifier identifier;
};

