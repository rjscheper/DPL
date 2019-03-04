#ifndef RECOGNIZER_H
#define RECOGNIZER_H

//#include <iostream>
//#include <ctype.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <fstream>
//#include <string>
using namespace std;

class Recognizer 
		{
		public:
		Lexeme *CurrentLexeme;
		Lexer *myLexer;
		Recognizer(Lexeme *curr, Lexer *l);
		bool check(GRAMTYPE type);
		Lexeme *advance(void);
		Lexeme *match(GRAMTYPE type);
		void matchNoAdvance(type);
		void printError(GRAMTYPE type);
		Lexeme *program(void);
		Lexeme *programExpressionList(void);
		Lexeme *expression(void);
		Lexeme *op(void);
		Lexeme *logicalOp(void);
		Lexeme *exprList(void);
		Lexeme *optExpressionList(void);
		bool programPending(void);
		bool operatorPending(void);
		bool logicalOpPending(void);
		bool expressionListPending(void);
		bool expressionPending(void);
		bool unaryPending(void);
		bool ifStatementPending(void);
		bool blockPending(void);
		bool statementPending(void);
		bool statementsPending(void);
		bool whileLoopPending(void);
		bool forLoopPending(void);
		bool varDefPending(void);
		bool functionDefPending(void);
		bool functionCallPending(void);
		bool parameterListPending(void);
		bool expressionListPending(void);
		bool expressionPending(void);
		bool unaryPending(void);
		Lexeme *unary(void);
		Lexeme *expressionList(void);
		Lexeme *ifStatement(void);
		Lexeme *optElse(void);
		Lexeme *block(void);
		Lexeme *statement(void);
		Lexeme *statements(void);
		Lexeme *whileLoop(void);
		Lexeme *forLoop(void);
		Lexeme *functionDef(void);
		Lexeme *functionCall(void);
		Lexeme *varDef(void);
		Lexeme *ParameterList(void);
		Lexeme *paramHelp(void);
		Lexeme *optParameterList(void);
		void prettyPrint(Lexeme *tree);
		
		};

#endif