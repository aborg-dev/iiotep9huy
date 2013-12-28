#pragma once

#include "Context.hpp"
#include "Identifier.hpp"
#include "Value.hpp"

#include <vector>

class Selector;
class Context;

class Expression
{
public:
	Expression() {}
	virtual ~Expression() {}

	virtual Value* evaluate(Context&);
};

class ExpressionSequence
{
public:
	ExpressionSequence() {}
	~ExpressionSequence() {}

	void addExpression(Expression* expression)
	{
		expressions.push_back(expression);
	}

	std::vector<Expression*> expressions;
};

class VariableExpression : public Expression
{
public:
	VariableExpression(Identifier identifier): identifier(identifier) {}
	virtual ~VariableExpression() {}

	virtual Value* evaluate(Context& context);

	Identifier identifier;
};

class SelectExpression : public Expression
{
public:
	SelectExpression(Identifier identifier, Selector* selector): 
		identifier(identifier), selector(selector) {}
	virtual ~SelectExpression() {}

	virtual Value* evaluate(Context& context);

	Identifier identifier;
	Selector* selector;
};

class ConstExpression : public Expression
{
public:
	ConstExpression(Value* value): value(value) {}
	virtual ~ConstExpression() {}

	virtual Value* evaluate(Context& context);

	Value* value;
};

enum class BinaryOperator
{
	PLUS = 1,
	MINUS = 2,
	DIV = 3,
	MOD = 4,
	DIVIDE = 5,
	MULTIPLY = 6,
	OR = 7,
	AND = 8,
	EQUAL = 9,
	NOT_EQUAL = 10,
	LESS = 11,
	GREATER = 12,
	LESS_EQUAL = 13,
	GREATER_EQUAL = 14
};

namespace binary_operator {

Value* plus(Value* leftOperand, Value* rightOperand);
Value* minus(Value* leftOperand, Value* rightOperand);
Value* div(Value* leftOperand, Value* rightOperand);
Value* mod(Value* leftOperand, Value* rightOperand);
Value* multiply(Value* leftOperand, Value* rightOperand);

Value* orOperator(Value* leftOperand, Value* rightOperand);
Value* andOperator(Value* leftOperand, Value* rightOperand);

Value* equal(Value* leftOperand, Value* rightOperand);
Value* notEqual(Value* leftOperand, Value* rightOperand);
Value* less(Value* leftOperand, Value* rightOperand);
Value* greater(Value* leftOperand, Value* rightOperand);
Value* lessEqual(Value* leftOperand, Value* rightOperand);
Value* greaterEqual(Value* leftOperand, Value* rightOperand);

}; // namespace binary_operator

class BinaryOperatorExpression : public Expression
{
public:
	BinaryOperatorExpression(Expression* leftOperand, 
														BinaryOperator Operator, 
														Expression* rightOperand) : leftOperand(leftOperand),
																												Operator(Operator),
																												rightOperand(rightOperand)
	{
	}
	virtual ~BinaryOperatorExpression() {}

	virtual Value* evaluate(Context& context);

	Expression* leftOperand;
	BinaryOperator Operator;
	Expression* rightOperand;
};

enum class UnaryOperator
{
	NEGATION = 1,
	COMPLEMENT = 2
};

namespace unary_operator {

Value* negation(Value* operand);
Value* complement(Value* operand);

}; // namespace unary_operator

class UnaryOperatorExpression : public Expression
{
public:
	UnaryOperatorExpression(Expression* operand, 
													UnaryOperator Operator) : operand(operand),
																										Operator(Operator)
	{
	}
	virtual ~UnaryOperatorExpression() {}

	virtual Value* evaluate(Context& context);

	Expression* operand;
	UnaryOperator Operator;
};
