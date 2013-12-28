#pragma once

#include "Value.hpp"
#include "Expression.hpp"
#include "Context.hpp"
#include "Field.hpp"
#include "Exceptions.hpp"

class Context;
class Expression;

class Type
{
public:
	Type() {}
	virtual ~Type() {}

	virtual Value* createNewValue(Context* context) = 0;
	virtual Value* createNewValue(Context* context, Value* value) = 0;
	virtual Identifier getType() = 0;
};

class NamedType : public Type
{
public:
	NamedType(Identifier identifier): identifier(identifier) {}
	virtual ~NamedType() {}

	virtual Value* createNewValue(Context* context);
	virtual Value* createNewValue(Context* context, Value* value);

	virtual Identifier getType() { return identifier; };

	Identifier identifier;
};

class ArrayType : public Type
{
public:
	ArrayType(Expression* expression, Type* arrayType);
	virtual ~ArrayType() {}

	virtual Value* createNewValue(Context* context);
	virtual Value* createNewValue(Context* context, Value* value);

	virtual Identifier getType() { return Identifier("ARRAY"); };

	Expression* expression;
	Type* arrayType;
};

class StructType : public Type
{
public:
	StructType(FieldListSequence* fieldListSequence);
	virtual ~StructType() {}

	virtual Value* createNewValue(Context* context);
	virtual Value* createNewValue(Context* context, Value* value);

	virtual Identifier getType() { return Identifier("RECORD"); };

	std::unordered_map<Identifier, Type*> fieldTypes;
};

class IntegerType : public Type
{
public:
	IntegerType() {}
	virtual ~IntegerType() {}

	virtual Value* createNewValue(Context* context)
	{
		return createNewValue(context, new IntegerValue(0));
	}
	virtual Value* createNewValue(Context* context, Value* value)
	{
		return value;
	}

	virtual Identifier getType() { return Identifier("INTEGER"); };
};

class BooleanType : public Type
{
public:
	BooleanType() {}
	virtual ~BooleanType() {}

	virtual Value* createNewValue(Context* context)
	{
		return createNewValue(context, new BooleanValue(false));
	}
	virtual Value* createNewValue(Context* context, Value* value)
	{
		return value;
	}

	virtual Identifier getType() { return Identifier("BOOLEAN"); };
};

class DoubleType : public Type
{
public:
	DoubleType() {}
	virtual ~DoubleType() {}

	virtual Value* createNewValue(Context* context)
	{
		return createNewValue(context, new DoubleValue(0.0));
	}
	virtual Value* createNewValue(Context* context, Value* value)
	{
		return value;
	}

	virtual Identifier getType() { return Identifier("DOUBLE"); };
};

class StringType : public Type
{
public:
	StringType() {}
	virtual ~StringType() {}

	virtual Value* createNewValue(Context* context)
	{
		return createNewValue(context, new StringValue(std::string("")));
	}
	virtual Value* createNewValue(Context* context, Value* value)
	{
		return value;
	}

	virtual Identifier getType() { return Identifier("STRING"); };
};
