#include "Type.hpp"

Value* NamedType::createNewValue(Context* context)
{
	createNewValue(context, nullptr);
}

Value* NamedType::createNewValue(Context* context, Value* value)
{
	return context->createValueOfType(identifier);
}

ArrayType::ArrayType(Expression* expression, Type* arrayType): 
	expression(expression), arrayType(arrayType) {}

Value* ArrayType::createNewValue(Context* context)
{
	createNewValue(context, nullptr);
}

Value* ArrayType::createNewValue(Context* context, Value* value)
{
	Value* arraySizeValue = expression->evaluate(*context);
	int arraySize = (dynamic_cast<IntegerValue*> (arraySizeValue))->value;
	ArrayValue* arrayValue = new ArrayValue(arraySize);
	for (int i = 0; i < arraySize; ++i)
	{
		arrayValue->array[i] = arrayType->createNewValue(context);
	}
	return arrayValue;
}

StructType::StructType(FieldListSequence* fieldListSequence)
{
	for (auto& fieldList : fieldListSequence->fieldLists)	
	{
		for (auto& identifier : fieldList->identifierList->identifiers)
		{
			fieldTypes[identifier] = fieldList->type;
		}
	}
}

Value* StructType::createNewValue(Context* context)
{
	return createNewValue(context, nullptr);
}

Value* StructType::createNewValue(Context* context, Value* value)
{
	StructValue* structValue = new StructValue();
	for (auto& field : fieldTypes)
	{
		structValue->fields[field.first] = field.second->createNewValue(context);
	}
	return structValue;
}
