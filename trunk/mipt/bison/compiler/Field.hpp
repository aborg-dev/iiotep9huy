#pragma once

#include "Identifier.hpp"
#include "Type.hpp"

#include <vector>

class Type;

class FieldList
{
public:
	FieldList(IdentifierList* identifierList, Type* type):
		identifierList(identifierList), type(type) {}
	~FieldList() {}

	IdentifierList* identifierList;
	Type* type;
};

class FieldListSequence
{
public:
	FieldListSequence() {}
	~FieldListSequence() {}

	void addFieldList(FieldList* fieldList)
	{
		fieldLists.push_back(fieldList);
	}

	std::vector<FieldList*>	fieldLists;
};