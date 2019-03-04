#ifndef EVALUATE_H
#define EVALUATE_H

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include "lexer.h"
#include "env.h"
using namespace std;

Lexeme *eval(Lexeme *tree, Lexeme *env);
void evalFuncDef(Lexeme *tree, Lexeme *env);
void evalVarDef(Lexeme *tree, Lexeme *env);
Lexeme *evalArgs(Lexeme *args, Lexeme *env);
Lexeme *apList(Lexeme *tree, bool b);
Lexeme *evalAnonDef(Lexeme *tree, Lexeme *env);
Lexeme *evalAnonCall(Lexeme *tree, Lexeme *env);
Lexeme *evalProgram(Lexeme *tree, Lexeme *env);
Lexeme *evalCall(Lexeme *tree, Lexeme *env);
Lexeme *evalBlock(Lexeme *tree, Lexeme *env);
Lexeme *evalSimpleOp(Lexeme *tree, Lexeme *env);
Lexeme *getFuncName(Lexeme *tree);
Lexeme *getFuncCallArgs(Lexeme *tree);
Lexeme *getClosureParams(Lexeme *closure);
Lexeme *getClosureBody(Lexeme *closure);
Lexeme *getClosureEnvironment(Lexeme *closure);
Lexeme *newIntegerLexeme(int val);
Lexeme *newStringLexeme(string s);
Lexeme *newBooleanLexeme(bool b);
Lexeme *evalPlus(Lexeme *tree, Lexeme *env);
Lexeme *evalMinus(Lexeme *tree, Lexeme *env);
Lexeme *evalUMinus(Lexeme *tree, Lexeme *env);
Lexeme *evalTimes(Lexeme *tree, Lexeme *env);
Lexeme *evalDivides(Lexeme *tree, Lexeme *env);
Lexeme *evalEqual(Lexeme *tree, Lexeme *env);
Lexeme *evalNot(Lexeme *tree, Lexeme *env);
Lexeme *evalGlue(Lexeme *tree, Lexeme *env);
Lexeme *evalAnd(Lexeme *tree, Lexeme *env);
Lexeme *evalOr(Lexeme *tree, Lexeme *env);
Lexeme *evalLessThan(Lexeme *tree, Lexeme *env);
Lexeme *evalGreaterThan(Lexeme *tree, Lexeme *env);
Lexeme *evalIf(Lexeme *tree, Lexeme *env);
Lexeme *evalWhile(Lexeme *tree, Lexeme *env);
void evalAssign(Lexeme *tree, Lexeme *env);
void printHelp(int i);
void printHelp(string s);
void printHelp(bool b);
void evalPrint(Lexeme *eargs);
Lexeme *isInt(Lexeme *eargs);
Lexeme *evalMakeArray(Lexeme *eargs);
Lexeme *evalGetArray(Lexeme *eargs);
void evalSetArray(Lexeme *eargs);
void viewArray(Lexeme *a);
void showArray(Lexeme *eargs);

#endif