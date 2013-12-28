#include "Value.hpp"
#include "Expression.hpp"

Value* Expression::evaluate(Context&)
{
}

Value* VariableExpression::evaluate(Context& context)
{
	return context.getVariableReference(identifier);
}

Value* SelectExpression::evaluate(Context& context)
{
	return selector->select(context.getVariableReference(identifier), &context);
}

Value* ConstExpression::evaluate(Context& context)
{
	return value;
}

Value* BinaryOperatorExpression::evaluate(Context& context)
{
	Value* leftValue = leftOperand->evaluate(context);
	Value* rightValue = rightOperand->evaluate(context);
	switch (Operator)
	{
		case BinaryOperator::PLUS:
			return leftValue->plus(rightValue);
			break;

		case BinaryOperator::MINUS:
			return leftValue->minus(rightValue);
			break;

		case BinaryOperator::DIV:
			return leftValue->div(rightValue);
			break;

		case BinaryOperator::MOD:
			return leftValue->mod(rightValue);
			break;

		case BinaryOperator::DIVIDE:
			return leftValue->divide(rightValue);
			break;

		case BinaryOperator::MULTIPLY:
			return leftValue->multiply(rightValue);
			break;

		case BinaryOperator::OR:
			return leftValue->orOperator(rightValue);
			break;

		case BinaryOperator::AND:
			return leftValue->andOperator(rightValue);
			break;

		case BinaryOperator::EQUAL:
			return leftValue->equal(rightValue);
			break;

		case BinaryOperator::NOT_EQUAL:
			return leftValue->notEqual(rightValue);
			break;

		case BinaryOperator::LESS:
			return leftValue->less(rightValue);
			break;

		case BinaryOperator::GREATER:
			return leftValue->greater(rightValue);
			break;

		case BinaryOperator::LESS_EQUAL:
			return leftValue->lessEqual(rightValue);
			break;

		case BinaryOperator::GREATER_EQUAL:
			return leftValue->greaterEqual(rightValue);
			break;
	}

	throw unknown_operator("Got unknown operator during BinaryOperatorExpression evaluation");
}

Value* UnaryOperatorExpression::evaluate(Context& context)
{
	Value* operandValue = operand->evaluate(context);
	switch (Operator)
	{
		case UnaryOperator::NEGATION:
			return operandValue->negation();
			break;

		case UnaryOperator::COMPLEMENT:
			return operandValue->complement();
			break;
	}

	throw unknown_operator("Got unknown operator during UnaryOperatorExpression evaluation");
}
