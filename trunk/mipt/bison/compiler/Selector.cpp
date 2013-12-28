#include "Selector.hpp"

Value* ArraySelector::select(Value* value, Context* context)
{
	ArrayValue* arrayValue = dynamic_cast<ArrayValue*> (value);

	if (context == nullptr)
	{
		throw std::logic_error("Can't get array element without context");
	}

	Value* indexValue = expression->evaluate(*context);
	IntegerValue* indexIntegerValue = dynamic_cast<IntegerValue*> (indexValue);
	if (!indexIntegerValue)
	{
		throw wrong_type("Wrong array index type");
	}

	int arrayIndex = indexIntegerValue->value;

	if (!arrayValue)
	{
		throw wrong_type("Got wrong type during selection of " 
			+ std::to_string(arrayIndex) + " index");
	}

	return arrayValue->array[arrayIndex];
}
