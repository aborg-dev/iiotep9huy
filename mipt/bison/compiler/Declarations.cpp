#include "Declarations.hpp"

Declarations::Declarations()
{
	procedureDeclarations = new ProcedureDeclarations();
}

Declarations::~Declarations()
{
}

void Declarations::declare(Context& context)
{
	constDeclarations.declare(context);
	typeDeclarations.declare(context);
	varDeclarations.declare(context);
	procedureDeclarations->declare(context);
}

void ConstDeclaration::declare(Context& context)
{
	//std::cerr << "Declaring const " << identifier << std::endl;
	context.declareAndSetVariable(identifier, expression->evaluate(context));
}

void ConstDeclarations::declare(Context& context)
{
	//std::cerr << "Declaring consts" << std::endl;
	for (auto& declaration : constDeclarations)
	{
		declaration.declare(context);
	}
}

void ConstDeclarations::addDeclaration(ConstDeclaration constDeclaration)
{
	constDeclarations.push_back(constDeclaration);
}

void TypeDeclaration::declare(Context& context)
{
	//std::cerr << "Declaring type " << identifier << std::endl;
	context.declareType(identifier, type);
}

void TypeDeclarations::declare(Context& context)
{
	//std::cerr << "Declaring types" << std::endl;
	for (auto& declaration : typeDeclarations)
	{
		declaration.declare(context);
	}
}

void TypeDeclarations::addDeclaration(TypeDeclaration typeDeclaration)
{
	typeDeclarations.push_back(typeDeclaration);
}

void VarDeclaration::declare(Context& context)
{
	//std::cerr << "Declaring variable " << varIdentifier << std::endl;
	context.declareVariable(varIdentifier, varType);
}

void VarDeclarations::declare(Context& context)
{
	//std::cerr << "Declaring variables" << std::endl;
	for (auto& declaration : varDeclarations)
	{
		declaration.declare(context);
	}
}

void VarDeclarations::addDeclaration(VarDeclaration varDeclaration)
{
	varDeclarations.push_back(varDeclaration);
}
