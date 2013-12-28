#include "Statement.hpp"

void Statement::execute(Context& context)
{
}

void StatementSequence::execute(Context& context)
{
	for (auto& statement : statements)
	{
		statement->execute(context);
	}
}

void StatementSequence::addStatement(Statement* statement)
{
	statements.push_back(statement);
}


void AssignmentStatement::execute(Context& context)
{
	context.setVariable(identifier, selector, expression->evaluate(context));
}

void IfStatement::execute(Context& context)
{
	//std::cerr << "Executing if statement" << std::endl;

	BooleanValue* expressionValue = dynamic_cast<BooleanValue*> (expression->evaluate(context));

	if (!expressionValue)
	{
		throw wrong_type("Wrong expression type in if statement");
	}

	if (expressionValue->value == true)
	{
		//std::cerr << "if statement: Expression is true" << std::endl;
		ifStatementSequence->execute(context);
	}
	else
	{
		//std::cerr << "if statement: Expression is false" << std::endl;
		elseStatementSequence->execute(context);
	}
}

void WhileStatement::execute(Context& context)
{
	while (true)
	{
		BooleanValue* expressionValue = dynamic_cast<BooleanValue*> (expression->evaluate(context));

		if (!expressionValue)
		{
			throw wrong_type("Wrong expression type in if statement");
		}

		if (expressionValue->value == false)
		{
			break;
		}

		statementSequence->execute(context);
	}
}

void ProcedureCallStatement::execute(Context& context)
{
	context.getProcedure(identifier)->call(actualParameters, context);
}

void ReadStatement::execute(Context& context)
{
	std::cin >> context.getVariableReference(identifier);
}

void WriteStatement::execute(Context& context)
{
	std::cout << context.getVariableReference(identifier) << std::endl;
}
