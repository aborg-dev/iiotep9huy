#pragma once

#include "Expression.hpp"
#include "Statement.hpp"
#include "Declarations.hpp"
#include "Type.hpp"

class ExpressionSequence;
class StatementSequence;
class Declarations;
class Type;

class FormalParameters;

class ProcedureHeading
{
public:
	ProcedureHeading(Identifier identifier);
	ProcedureHeading(Identifier identifier, FormalParameters* formalParameters);
	~ProcedureHeading();

	Identifier identifier;
	FormalParameters* formalParameters;
};

class ProcedureBody
{
public:
	ProcedureBody();
	ProcedureBody(Declarations* declarations);
	ProcedureBody(Declarations* declarations, StatementSequence* procedureBlock);
	~ProcedureBody();

	Declarations* declarations;
	StatementSequence* procedureBlock;
};

class ProcedureDeclaration
{
public:
	ProcedureDeclaration(ProcedureHeading* procedureHeading, ProcedureBody* procedureBody):
		procedureHeading(procedureHeading), procedureBody(procedureBody) {}
	~ProcedureDeclaration() {}

	void declare(Context& context);

	ProcedureHeading* procedureHeading;
	ProcedureBody* procedureBody;
};

class ProcedureDeclarations
{
public:
	void declare(Context& context);

	void addDeclaration(ProcedureDeclaration* procedureDeclaration);

private:
	std::vector<ProcedureDeclaration*> procedureDelcarations; 
};

class ActualParameters;

class Procedure
{
public:
	Procedure(ProcedureHeading* procedureHeading, ProcedureBody* procedureBody):
		procedureHeading(procedureHeading), procedureBody(procedureBody) {}
	~Procedure() {}

	void call(ActualParameters* actualParameters, Context& context);

	ProcedureHeading* procedureHeading;
	ProcedureBody* procedureBody;
};

class ActualParameters
{
public:
	ActualParameters();
	ActualParameters(ExpressionSequence* expressionSequence);
	~ActualParameters();

	ExpressionSequence* expressionSequence;
};

class FormalParametersSection
{
public:
	FormalParametersSection(IdentifierList* identifierList, Type* type, bool isVar):
		identifierList(identifierList), type(type), isVar(isVar) {}
	~FormalParametersSection() {}

	IdentifierList* identifierList;
	Type* type;
	bool isVar;
};

class FormalParametersSectionSequence
{
public:
	FormalParametersSectionSequence() {}
	~FormalParametersSectionSequence() {}

	void addFormalParametersSection(FormalParametersSection* formalParametersSection)
	{
		formalParametersSections.push_back(formalParametersSection);
	}

	std::vector<FormalParametersSection*> formalParametersSections;
};

class FormalParameters
{
public:
	FormalParameters(FormalParametersSectionSequence* formalParametersSectionSequence
		= new FormalParametersSectionSequence()):
		formalParametersSectionSequence(formalParametersSectionSequence) {}
	~FormalParameters() {}

	FormalParametersSectionSequence* formalParametersSectionSequence;
};