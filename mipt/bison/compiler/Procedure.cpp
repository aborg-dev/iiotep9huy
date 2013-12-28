#include "Procedure.hpp"

ProcedureHeading::ProcedureHeading(Identifier identifier): 
	ProcedureHeading(identifier, new FormalParameters())
{
}

ProcedureHeading::ProcedureHeading(Identifier identifier, FormalParameters* formalParameters):
	identifier(identifier), formalParameters(formalParameters) 
{
}

ProcedureHeading::~ProcedureHeading() {}

ProcedureBody::ProcedureBody():
	ProcedureBody(new Declarations())
{
}

ProcedureBody::ProcedureBody(Declarations* declarations):
	ProcedureBody(declarations, new StatementSequence())
{
}

ProcedureBody::ProcedureBody(Declarations* declarations, StatementSequence* procedureBlock):
	declarations(declarations), procedureBlock(procedureBlock)
{
}

ProcedureBody::~ProcedureBody() {}

void ProcedureDeclaration::declare(Context& context)
{
	Identifier identifier = procedureHeading->identifier;
	//std::cerr << "Declaring procedure " << identifier << std::endl;
	context.declareProcedure(identifier, new Procedure(procedureHeading, procedureBody));
}

void ProcedureDeclarations::declare(Context& context)
{
	//std::cerr << "Declaring procedures" << std::endl;
	for (auto& declaration : procedureDelcarations)
	{
		declaration->declare(context);
	}
}

void ProcedureDeclarations::addDeclaration(ProcedureDeclaration* procedureDeclaration)
{
	procedureDelcarations.push_back(procedureDeclaration);
}

void Procedure::call(ActualParameters* actualParameters, Context& context)
{
	//std::cerr << "calling procedure " << procedureHeading->identifier << std::endl;

	/* TODO: add function to context to go deeper */
	context.currentLevel++;
	std::vector<Expression*>& actualParametersExpressions = 
		actualParameters->expressionSequence->expressions;

	int parameterIndex = 0;
	std::vector<FormalParametersSection*>& formalParametersSections =
		procedureHeading->formalParameters->formalParametersSectionSequence->formalParametersSections;

	for(auto& section : formalParametersSections)
	{
		for(auto& identifier : section->identifierList->identifiers)	
		{
			if (parameterIndex >= actualParametersExpressions.size())
			{
				throw wrong_arguments_number("Wrong agruments number in procedure " 
					+ procedureHeading->identifier);
			}
			if (section->isVar)
			{
				Value* variable = 
					actualParametersExpressions[parameterIndex]->evaluate(context);
				context.declareAndReferenceVariable(identifier, variable);
				++parameterIndex;
			}
			else
			{
				context.declareAndSetVariable(identifier, 
					actualParametersExpressions[parameterIndex]->evaluate(context));
				++parameterIndex;
			}
		}
	}

	procedureBody->declarations->declare(context);
	procedureBody->procedureBlock->execute(context);
	context.removeObsoleteVariables();
	context.restoreShadowedVariables();

	context.currentLevel--;
}

ActualParameters::ActualParameters(): 
	ActualParameters(new ExpressionSequence())
{
}

ActualParameters::ActualParameters(ExpressionSequence* expressionSequence):
	expressionSequence(expressionSequence)
{
}

ActualParameters::~ActualParameters() {}
