#pragma once

#include <vector>
#include <iostream>

#include "Identifier.hpp"
#include "Context.hpp"
#include "Expression.hpp"
#include "Type.hpp"
#include "Procedure.hpp"

class Type;
class ProcedureDeclarations;

class ConstDeclaration
{
public:
	ConstDeclaration(Identifier identifier, Expression* expression):
		identifier(identifier), expression(expression) {}
	~ConstDeclaration() {}

	void declare(Context& context);

private:
	Identifier identifier;
	Expression* expression;
};

class ConstDeclarations
{
public:
	void declare(Context& context);

	void addDeclaration(ConstDeclaration constDeclaration);

private:
	std::vector<ConstDeclaration> constDeclarations;
};

class TypeDeclaration
{
public:
	TypeDeclaration(Identifier identifier, Type* type): 
		identifier(identifier), type(type) {}
	~TypeDeclaration() {}

	void declare(Context& context);

private:
	Identifier identifier;
	Type* type;
};

class TypeDeclarations
{
public:
	void declare(Context& context);

	void addDeclaration(TypeDeclaration typeDeclaration);

private:
	std::vector<TypeDeclaration> typeDeclarations;
};


class VarDeclaration
{
public:
	VarDeclaration(Identifier varIdentifier, Type* varType): 
		varIdentifier(varIdentifier), varType(varType) {}
	~VarDeclaration() {}

	void declare(Context& context);

private:
	Identifier varIdentifier;
	Type* varType;
};

class VarDeclarations
{
public:
	void declare(Context& context);

	void addDeclaration(VarDeclaration varDeclaration);

private:
	std::vector<VarDeclaration> varDeclarations; 
};

class Declarations
{
public:
	Declarations();
	~Declarations();
	void declare(Context& context);

/* TODO: make everything with pointers */
	ConstDeclarations constDeclarations;
	TypeDeclarations typeDeclarations;
	VarDeclarations varDeclarations;
	ProcedureDeclarations* procedureDeclarations;
};

