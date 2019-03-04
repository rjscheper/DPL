#ifndef ENV_H
#define ENV_H

#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include "lexer.h"
#include "env.h"
using namespace std;

Lexeme *cons(GRAMTYPE type, Lexeme *carValue, Lexeme *cdrValue);
Lexeme *car(Lexeme *cell);
Lexeme *cdr(Lexeme *cell);
void setCar(Lexeme *cell, Lexeme *value);
void setCdr(Lexeme *cell, Lexeme *value);
bool sameVariable(Lexeme *x, Lexeme *y);
Lexeme *create(void);
Lexeme *makeTable(Lexeme *variables, Lexeme *values);
Lexeme *extendEnv(Lexeme *env, Lexeme *variables, Lexeme *values);
Lexeme *insertEnv(Lexeme *variable, Lexeme *value, Lexeme *env);
Lexeme *lookupEnv(Lexeme *variable, Lexeme *env);
void updateEnv(Lexeme *env, Lexeme *variable, Lexeme *value);
void displayEnv(Lexeme *env);

#endif