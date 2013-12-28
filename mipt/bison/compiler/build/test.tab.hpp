/* A Bison parser, made by GNU Bison 3.0.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_TEST_TAB_HPP_INCLUDED
# define YY_YY_TEST_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    MODULE = 258,
    PROCEDURE = 259,
    BEGIN_ = 260,
    END = 261,
    CONST = 262,
    TYPE = 263,
    VAR = 264,
    RECORD = 265,
    ARRAY = 266,
    OF = 267,
    WHILE = 268,
    DO = 269,
    ELSE = 270,
    IF = 271,
    ELSIF = 272,
    THEN = 273,
    IDENTIFIER = 274,
    INTEGER = 275,
    DOUBLE = 276,
    BOOLEAN = 277,
    STRING = 278,
    ASSGNOP = 279,
    NE = 280,
    LE = 281,
    GE = 282,
    OR = 283,
    DIV = 284,
    MOD = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 23 "/home/iiotep9huy/Templates/svn/iiotep9huy/mipt/bison/compiler/test.ypp" /* yacc.c:1909  */

  Module* module;

  Identifier* identifier;
  IdentifierList* identifierList;
  FieldList* fieldList;
  FieldListSequence* fieldListSequence;
  Value* value;
  Type* valueType;
  ArrayType* arrayType;
  StructType* structType;

  Declarations* declarations;

  IntegerValue* integer;
  DoubleValue* double_;
  StringValue* string;
  BooleanValue* boolean;

  Expression* expression;
  ExpressionSequence* expressionSequence;
  Selector* selector;

  Statement* statement;
  AssignmentStatement* assignmentStatement;
  WhileStatement* whileStatement;
  IfStatement* ifStatement;

  ProcedureCallStatement* procedureCallStatement;
  ActualParameters* actualParameters;

  ProcedureDeclaration* procedureDeclaration;

  ProcedureHeading* procedureHeading;
  FormalParameters* formalParameters;
  FormalParametersSection* formalParametersSection;
  FormalParametersSectionSequence* formalParametersSectionSequence;

  ProcedureBody* procedureBody;

  StatementSequence* statementSequence;

#line 128 "test.tab.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TEST_TAB_HPP_INCLUDED  */
