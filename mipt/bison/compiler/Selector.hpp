#pragma once

#include "Value.hpp"
#include "Identifier.hpp"
#include "Exceptions.hpp"
#include "Expression.hpp"
#include "Context.hpp"

#include <iostream>

class Context;
class Expression;

class Selector
{
public:
	Selector() {}
	virtual ~Selector() {}

	virtual Value* select(Value* value, Context* context)
	{
		return value;
	}
};

class NestedSelector : public Selector
{
public:
	NestedSelector(Selector* currentLayerSelector, Selector* nextLayerSelector):
		currentLayerSelector(currentLayerSelector), nextLayerSelector(nextLayerSelector) {}
	virtual ~NestedSelector() {}

	virtual Value* select(Value* value, Context* context)
	{
		Value* selectedValue = currentLayerSelector->select(value, context);
		return nextLayerSelector->select(selectedValue, context);
	}

	Selector* currentLayerSelector;
	Selector* nextLayerSelector;
};

class FieldSelector : public Selector
{
public:
	FieldSelector(Identifier identifier): identifier(identifier) {}
	virtual ~FieldSelector() {}

	virtual Value* select(Value* value, Context* context)
	{
		StructValue* structValue = dynamic_cast<StructValue*> (value);

		if (!structValue)
		{
			throw wrong_type("Got wrong type during selection of " + identifier + " field");
		}

		return structValue->fields[identifier];
	}

	Identifier identifier;
};

class ArraySelector : public Selector
{
public:
	ArraySelector(Expression* expression): expression(expression) {}
	virtual ~ArraySelector() {}

	virtual Value* select(Value* value, Context* context);

	Expression* expression;
};