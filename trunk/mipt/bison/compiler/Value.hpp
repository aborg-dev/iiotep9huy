#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <istream>
#include <ostream>

#include "Identifier.hpp"
#include "Exceptions.hpp"

class Value
{
public:
	Value(bool isConst = false): isConst(isConst) {}
	virtual ~Value() {}

	virtual void setValue(Value* value) = 0;

	virtual Value* copy() = 0;

	virtual void print(std::ostream& os) = 0;
	virtual void read(std::istream& is) = 0;

	friend std::ostream& operator<< (std::ostream& os, Value* v)
	{
		v->print(os);
	  return os;
	}

	friend std::istream& operator>> (std::istream& is, Value* v)
	{
		v->read(is);
	  return is;
	}

	virtual Identifier getType() = 0;

	virtual Value* negation() 
	{ 
		throw wrong_type("Operand of negation operator has incorrect type " + getType());
	}

	virtual Value* complement() 
	{ 
		throw wrong_type("Operand of complement operator has incorrect type " + getType());
	}

	virtual Value* plus(Value* operand) 
	{ 
		throw wrong_type("Left operand of plus operator has incorrect type " + getType());
	}

	virtual Value* minus(Value* operand)
	{ 
		throw wrong_type("Left operand of minus operator has incorrect type " + getType());
	}

	virtual Value* div(Value* operand)
	{ 
		throw wrong_type("Left operand of div operator has incorrect type " + getType());
	}

	virtual Value* divide(Value* operand)
	{ 
		throw wrong_type("Left operand of divide operator has incorrect type " + getType());
	}

	virtual Value* mod(Value* operand)
	{ 
		throw wrong_type("Left operand of mod operator has incorrect type " + getType());
	}

	virtual Value* multiply(Value* operand)
	{ 
		throw wrong_type("Left operand of multiply operator has incorrect type " + getType());
	}

	virtual Value* orOperator(Value* operand)
	{ 
		throw wrong_type("Left operand of or operator has incorrect type " + getType());
	}

	virtual Value* andOperator(Value* operand)
	{ 
		throw wrong_type("Left operand of and operator has incorrect type " + getType());
	}

	virtual Value* equal(Value* operand)
	{ 
		throw wrong_type("Left operand of equal operator has incorrect type " + getType());
	}

	virtual Value* notEqual(Value* operand)
	{ 
		throw wrong_type("Left operand of not equal operator has incorrect type " + getType());
	}

	virtual Value* less(Value* operand)
	{ 
		throw wrong_type("Left operand of less operator has incorrect type " + getType());
	}

	virtual Value* greater(Value* operand)
	{ 
		throw wrong_type("Left operand of greater operator has incorrect type " + getType());
	}

	virtual Value* lessEqual(Value* operand)
	{ 
		throw wrong_type("Left operand of less equal operator has incorrect type " + getType());
	}

	virtual Value* greaterEqual(Value* operand)
	{ 
		throw wrong_type("Left operand of greater equal operator has incorrect type " + getType());
	}

	bool isConst;
};


class ArrayValue : public Value
{
public:
	ArrayValue(int arraySize)
	{
		array.resize(arraySize);
	}
	virtual ~ArrayValue() {}
	
	virtual void setValue(Value* value)
	{
		ArrayValue* arrayValue = dynamic_cast<ArrayValue*> (value);

		if (!arrayValue)
		{
			throw wrong_type("Wrong assignment type");
		}

		for (int i = 0; i < array.size(); ++i)
		{
			array[i]->setValue(arrayValue->array[i]);
		}
	}

	virtual Value* copy()
	{
		ArrayValue* newValue = new ArrayValue(array.size());	
		for (int i = 0; i < array.size(); ++i)
		{
			newValue->array[i] = array[i]->copy();
		}
		return newValue;
	}

	virtual void print(std::ostream& os)
	{
		os << "[";
		for (int i = 0; i < array.size(); ++i)
		{
			if (i != 0)
				os << ", ";
			os << array[i];
		}
		os << "]";
	}

	virtual void read(std::istream& is)
	{
		for (int i = 0; i < array.size(); ++i)
		{
			is >> array[i];
		}
	}

	virtual Identifier getType() { return Identifier("ARRAY"); }

	std::vector<Value*> array;
};

class StructValue : public Value
{
public:
	StructValue() {}
	virtual ~StructValue() {}

	virtual void setValue(Value* value)
	{
		StructValue* structValue = dynamic_cast<StructValue*> (value);

		if (!structValue)
		{
			throw wrong_type("Wrong assignment type");
		}

		for (auto& field : fields)
		{
			if (!structValue->hasField(field.first))
			{
				throw wrong_type("Wrong assignment type, field " + field.first + " is missing");
			}
			field.second->setValue(structValue->fields[field.first]);
		}
	}

	virtual Value* copy()
	{
		StructValue* newValue = new StructValue();
		for (auto& field : fields)
		{
			newValue->fields[field.first] = field.second->copy();
		}
		return newValue;
	}

	virtual void print(std::ostream& os)
	{
		os << '{';
		bool isFirst = true;
		for (auto& field : fields)
		{
			if (!isFirst)
				os << ", ";
			isFirst = false;
			os << field.first << " : " <<	field.second;
		}
		os << '}';
	}

	virtual void read(std::istream& is)
	{
		throw wrong_type("Read operation is unsupported");
	}

	bool hasField(Identifier identifier)
	{
		return (fields.find(identifier) != fields.end());	
	}

	virtual Identifier getType() { return Identifier("RECORD"); }

	std::unordered_map<Identifier, Value*> fields;
};

class IntegerValue : public Value
{
public:
	explicit IntegerValue(int value): value(value) {}
	virtual ~IntegerValue() {}

	virtual void setValue(Value* value)
	{
		IntegerValue* integerValue = dynamic_cast<IntegerValue*> (value);

		if (!integerValue)
		{
			throw wrong_type("Wrong assignment type");
		}

		this->value = integerValue->value;
	}

	virtual Value* copy()
	{
		return new IntegerValue(value);
	}

	virtual void print(std::ostream& os)
	{
		os << value;
	}

	virtual void read(std::istream& is)
	{
		is >> value;
	}

	virtual Identifier getType() { return Identifier("INTEGER"); }

	virtual Value* negation();
	virtual Value* complement();

	virtual Value* plus(Value* operand);
	virtual Value* minus(Value* operand);
	virtual Value* div(Value* operand);
	virtual Value* mod(Value* operand);
	virtual Value* divide(Value* operand);
	virtual Value* multiply(Value* operand);

	virtual Value* orOperator(Value* operand);
	virtual Value* andOperator(Value* operand);

	virtual Value* equal(Value* operand);
	virtual Value* notEqual(Value* operand);
	virtual Value* less(Value* operand);
	virtual Value* greater(Value* operand);
	virtual Value* lessEqual(Value* operand);
	virtual Value* greaterEqual(Value* operand);

	int value;
};

class DoubleValue : public Value
{
public:
	explicit DoubleValue(double value): value(value) {}
	virtual ~DoubleValue() {}

	virtual void setValue(Value* value)
	{
		DoubleValue* doubleValue = dynamic_cast<DoubleValue*> (value);

		if (!doubleValue)
		{
			throw wrong_type("Wrong assignment type");
		}

		this->value = doubleValue->value;
	}

	virtual Value* copy()
	{
		return new DoubleValue(value);
	}

	virtual void print(std::ostream& os)
	{
		os << value;
	}

	virtual void read(std::istream& is)
	{
		is >> value;
	}

	virtual Identifier getType() { return Identifier("DOUBLE"); }

	virtual Value* negation();

	virtual Value* plus(Value* operand);
	virtual Value* minus(Value* operand);
	virtual Value* divide(Value* operand);
	virtual Value* multiply(Value* operand);

	virtual Value* equal(Value* operand);
	virtual Value* notEqual(Value* operand);
	virtual Value* less(Value* operand);
	virtual Value* greater(Value* operand);
	virtual Value* lessEqual(Value* operand);
	virtual Value* greaterEqual(Value* operand);

	double value;
};

class BooleanValue : public Value
{
public:
	explicit BooleanValue(bool value): value(value) {}
	virtual ~BooleanValue() {}

	virtual void setValue(Value* value)
	{
		BooleanValue* booleanValue = dynamic_cast<BooleanValue*> (value);

		if (!booleanValue)
		{
			throw wrong_type("Wrong assignment type");
		}

		this->value = booleanValue->value;
	}

	virtual Value* copy()
	{
		return new BooleanValue(value);
	}

	virtual void print(std::ostream& os)
	{
		os << value;
	}

	virtual void read(std::istream& is)
	{
		is >> value;
	}

	virtual Identifier getType() { return Identifier("BOOLEAN"); }

	virtual Value* negation();
	virtual Value* complement();

	virtual Value* plus(Value* operand);
	virtual Value* multiply(Value* operand);

	virtual Value* orOperator(Value* operand);
	virtual Value* andOperator(Value* operand);

	virtual Value* equal(Value* operand);
	virtual Value* notEqual(Value* operand);
	virtual Value* less(Value* operand);
	virtual Value* greater(Value* operand);
	virtual Value* lessEqual(Value* operand);
	virtual Value* greaterEqual(Value* operand);

	bool value;
};

class StringValue : public Value
{
public:
	explicit StringValue(std::string value): value(value) {}
	virtual ~StringValue() {}

	virtual void setValue(Value* value)
	{
		StringValue* stringValue = dynamic_cast<StringValue*> (value);

		if (!stringValue)
		{
			throw wrong_type("Wrong assignment type");
		}

		this->value = stringValue->value;
	}

	virtual Value* copy()
	{
		return new StringValue(value);
	}

	virtual void print(std::ostream& os)
	{
		os << value;
	}

	virtual void read(std::istream& is)
	{
		is >> value;
	}

	virtual Identifier getType() { return Identifier("STRING"); }

	virtual Value* plus(Value* operand);
	virtual Value* multiply(Value* operand);

	virtual Value* equal(Value* operand);
	virtual Value* notEqual(Value* operand);
	virtual Value* less(Value* operand);
	virtual Value* greater(Value* operand);
	virtual Value* lessEqual(Value* operand);
	virtual Value* greaterEqual(Value* operand);

	std::string value;
};
