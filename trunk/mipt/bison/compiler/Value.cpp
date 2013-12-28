#include "Value.hpp"

Value* IntegerValue::negation()
{
	return new IntegerValue(-(this->value));
}

Value* IntegerValue::complement()
{
	return new IntegerValue(~(this->value));
}

Value* IntegerValue::plus(Value* operand) 
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new IntegerValue(this->value + operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new IntegerValue(this->value + operandValue->value);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value + operandValue->value);
	}

	throw wrong_type("Right operand of plus operator has incorrect type " + operand->getType());
}

Value* IntegerValue::minus(Value* operand) 
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new IntegerValue(this->value - operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new IntegerValue(this->value - operandValue->value);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value - operandValue->value);
	}

	throw wrong_type("Right operand of minus operator has incorrect type " + operand->getType());
}

Value* IntegerValue::div(Value* operand) 
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		if (operandValue->value == 0)
		{
			throw std::out_of_range("Division by zero in div operator");
		}
		return new IntegerValue(this->value / operandValue->value);
	}

	throw wrong_type("Right operand of div operator has incorrect type " + operand->getType());
}

Value* IntegerValue::mod(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		if (operandValue->value == 0)
		{
			throw std::out_of_range("Division by zero in mod operator");
		}
		return new IntegerValue(this->value % operandValue->value);
	}

	throw wrong_type("Right operand of mod operator has incorrect type " + operand->getType());
}

Value* IntegerValue::divide(Value* operand) 
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		if (operandValue->value == 0)
		{
			throw std::out_of_range("Division by zero in div operator");
		}
		return new DoubleValue(double(this->value) / operandValue->value);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		if (operandValue->value == 0)
		{
			throw std::out_of_range("Division by zero in div operator");
		}
		return new DoubleValue(this->value / operandValue->value);
	}

	throw wrong_type("Right operand of div operator has incorrect type " + operand->getType());
}

Value* IntegerValue::multiply(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new IntegerValue(this->value * operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new IntegerValue(this->value * operandValue->value);
	}
	else
	if (operand->getType() == "STRING")
	{
		return operand->multiply(this);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value * operandValue->value);
	}

	throw wrong_type("Right operand of multiply operator has incorrect type " + operand->getType());
}

Value* IntegerValue::orOperator(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new IntegerValue(this->value | operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new IntegerValue(this->value | operandValue->value);
	}

	throw wrong_type("Right operand of or operator has incorrect type " + operand->getType());
}

Value* IntegerValue::andOperator(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new IntegerValue(this->value & operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new IntegerValue(this->value & operandValue->value);
	}

	throw wrong_type("Right operand of and operator has incorrect type " + operand->getType());
}

Value* IntegerValue::equal(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value == operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value == operandValue->value);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new BooleanValue(this->value == operandValue->value);
	}

	throw wrong_type("Right operand of equal operator has incorrect type " + operand->getType());
}

Value* IntegerValue::notEqual(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value != operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value != operandValue->value);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new BooleanValue(this->value != operandValue->value);
	}

	throw wrong_type("Right operand of not equal operator has incorrect type " + operand->getType());
}

Value* IntegerValue::less(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value < operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value < operandValue->value);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new BooleanValue(this->value < operandValue->value);
	}

	throw wrong_type("Right operand of less operator has incorrect type " + operand->getType());
}

Value* IntegerValue::greater(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value > operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value > operandValue->value);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new BooleanValue(this->value > operandValue->value);
	}

	throw wrong_type("Right operand of greater operator has incorrect type " + operand->getType());
}

Value* IntegerValue::lessEqual(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value <= operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value <= operandValue->value);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new BooleanValue(this->value <= operandValue->value);
	}

	throw wrong_type("Right operand of less equal operator has incorrect type " + operand->getType());
}

Value* IntegerValue::greaterEqual(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value >= operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value >= operandValue->value);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new BooleanValue(this->value >= operandValue->value);
	}

	throw wrong_type("Right operand of greater equal operator has incorrect type " + operand->getType());
}

Value* DoubleValue::negation()
{
	return new DoubleValue(-(this->value));
}

Value* DoubleValue::plus(Value* operand) 
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return operandValue->plus(this);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value + operandValue->value);
	}

	throw wrong_type("Right operand of plus operator has incorrect type " + operand->getType());
}

Value* DoubleValue::minus(Value* operand) 
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new DoubleValue(this->value - operandValue->value);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value - operandValue->value);
	}

	throw wrong_type("Right operand of minus operator has incorrect type " + operand->getType());
}

Value* DoubleValue::divide(Value* operand) 
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		if (operandValue->value == 0)
		{
			throw std::out_of_range("Division by zero in div operator");
		}
		return new DoubleValue(this->value / operandValue->value);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		if (operandValue->value == 0)
		{
			throw std::out_of_range("Division by zero in div operator");
		}
		return new DoubleValue(this->value / operandValue->value);
	}

	throw wrong_type("Right operand of div operator has incorrect type " + operand->getType());
}

Value* DoubleValue::multiply(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return operandValue->multiply(this);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value * operandValue->value);
	}

	throw wrong_type("Right operand of multiply operator has incorrect type " + operand->getType());
}

Value* DoubleValue::equal(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return operandValue->equal(this);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value == operandValue->value);
	}

	throw wrong_type("Right operand of equal operator has incorrect type " + operand->getType());
}

Value* DoubleValue::notEqual(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return operandValue->notEqual(this);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value != operandValue->value);
	}

	throw wrong_type("Right operand of not equal operator has incorrect type " + operand->getType());
}

Value* DoubleValue::less(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return operandValue->greater(this);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value < operandValue->value);
	}

	throw wrong_type("Right operand of less operator has incorrect type " + operand->getType());
}

Value* DoubleValue::greater(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return operandValue->less(this);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value > operandValue->value);
	}

	throw wrong_type("Right operand of greater operator has incorrect type " + operand->getType());
}

Value* DoubleValue::lessEqual(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return operandValue->greaterEqual(this);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value <= operandValue->value);
	}

	throw wrong_type("Right operand of less equal operator has incorrect type " + operand->getType());
}

Value* DoubleValue::greaterEqual(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return operandValue->lessEqual(this);
	}
	else
	if (operand->getType() == "DOUBLE")
	{
		DoubleValue* operandValue = dynamic_cast<DoubleValue*> (operand);
		return new DoubleValue(this->value >= operandValue->value);
	}

	throw wrong_type("Right operand of greater equal operator has incorrect type " + operand->getType());
}

Value* BooleanValue::negation()
{
	return new BooleanValue(-(this->value));
}

Value* BooleanValue::complement()
{
	return new BooleanValue(!(this->value));
}

Value* BooleanValue::plus(Value* operand) 
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new IntegerValue(this->value + operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new IntegerValue(this->value + operandValue->value);
	}

	throw wrong_type("Right operand of plus operator has incorrect type " + operand->getType());
}

Value* BooleanValue::multiply(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new IntegerValue(this->value * operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new IntegerValue(this->value * operandValue->value);
	}

	throw wrong_type("Right operand of multiply operator has incorrect type " + operand->getType());
}

Value* BooleanValue::orOperator(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new IntegerValue(this->value | operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value || operandValue->value);
	}

	throw wrong_type("Right operand of or operator has incorrect type " + operand->getType());
}

Value* BooleanValue::andOperator(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new IntegerValue(this->value & operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value && operandValue->value);
	}

	throw wrong_type("Right operand of and operator has incorrect type " + operand->getType());
}

Value* BooleanValue::equal(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value == operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value == operandValue->value);
	}

	throw wrong_type("Right operand of equal operator has incorrect type " + operand->getType());
}

Value* BooleanValue::notEqual(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value != operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value != operandValue->value);
	}

	throw wrong_type("Right operand of not equal operator has incorrect type " + operand->getType());
}

Value* BooleanValue::less(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value < operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value < operandValue->value);
	}

	throw wrong_type("Right operand of less operator has incorrect type " + operand->getType());
}

Value* BooleanValue::greater(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value > operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value > operandValue->value);
	}

	throw wrong_type("Right operand of greater operator has incorrect type " + operand->getType());
}

Value* BooleanValue::lessEqual(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value <= operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value <= operandValue->value);
	}

	throw wrong_type("Right operand of less equal operator has incorrect type " + operand->getType());
}

Value* BooleanValue::greaterEqual(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);
		return new BooleanValue(this->value >= operandValue->value);
	}
	else
	if (operand->getType() == "BOOLEAN")
	{
		BooleanValue* operandValue = dynamic_cast<BooleanValue*> (operand);
		return new BooleanValue(this->value >= operandValue->value);
	}

	throw wrong_type("Right operand of greater equal operator has incorrect type " + operand->getType());
}

Value* StringValue::plus(Value* operand) 
{ 
	if (operand->getType() == "STRING")
	{
		StringValue* operandValue = dynamic_cast<StringValue*> (operand);
		return new StringValue(this->value + operandValue->value);
	}

	throw wrong_type("Right operand of plus operator has incorrect type " + operand->getType());
}

Value* StringValue::multiply(Value* operand)
{ 
	if (operand->getType() == "INTEGER")
	{
		IntegerValue* operandValue = dynamic_cast<IntegerValue*> (operand);

		std::string resultString;
		for (int i = 0; i < operandValue->value; ++i)
		{
			resultString += this->value;
		}

		return new StringValue(resultString);
	}

	throw wrong_type("Right operand of multiply operator has incorrect type " + operand->getType());
}

Value* StringValue::equal(Value* operand)
{
	if (operand->getType() == "STRING")
	{
		StringValue* operandValue = dynamic_cast<StringValue*> (operand);
		return new BooleanValue(this->value == operandValue->value);
	}

	throw wrong_type("Right operand of equal operator has incorrect type " + operand->getType());
}

Value* StringValue::notEqual(Value* operand)
{ 
	if (operand->getType() == "STRING")
	{
		StringValue* operandValue = dynamic_cast<StringValue*> (operand);
		return new BooleanValue(this->value != operandValue->value);
	}

	throw wrong_type("Right operand of not equal operator has incorrect type " + operand->getType());
}

Value* StringValue::less(Value* operand)
{ 
	if (operand->getType() == "STRING")
	{
		StringValue* operandValue = dynamic_cast<StringValue*> (operand);
		return new BooleanValue(this->value < operandValue->value);
	}

	throw wrong_type("Right operand of less operator has incorrect type " + operand->getType());
}

Value* StringValue::greater(Value* operand)
{ 
	if (operand->getType() == "STRING")
	{
		StringValue* operandValue = dynamic_cast<StringValue*> (operand);
		return new BooleanValue(this->value > operandValue->value);
	}

	throw wrong_type("Right operand of greater operator has incorrect type " + operand->getType());
}

Value* StringValue::lessEqual(Value* operand)
{ 
	if (operand->getType() == "STRING")
	{
		StringValue* operandValue = dynamic_cast<StringValue*> (operand);
		return new BooleanValue(this->value <= operandValue->value);
	}

	throw wrong_type("Right operand of less equal operator has incorrect type " + operand->getType());
}

Value* StringValue::greaterEqual(Value* operand)
{ 
	if (operand->getType() == "STRING")
	{
		StringValue* operandValue = dynamic_cast<StringValue*> (operand);
		return new BooleanValue(this->value >= operandValue->value);
	}

	throw wrong_type("Right operand of greater equal operator has incorrect type " + operand->getType());
}

