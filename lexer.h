#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;

typedef enum gramtypes {IF, AND, OR, WHILE, FOR, ELSE, EQUAL, VARIABLE, INTEGER, STRING, PROGRAM, 
    FUNCTION, VAR, ENDofINPUT, COMMENT, LIST, FUNCTIONCALL, STATEMENT, STATEMENTS, BOOLEAN, OPAREN, CPAREN, COMMA, SEMICOLON, 
    UNKNOWN, ASSIGN, PLUS, TIMES, MINUS, DIVIDES, LESSTHAN, GREATERTHAN, OPERATOR, LOGICALOPERATOR, UNARY, DOT, GLUE, BINARY, 
    NOT, NEWLINE, NIL, UMINUS, ANONYMOUS, ACALL, ENV, OBRACE, CBRACE, OBRACKET, CBRACKET, TABLE, CLOSURE, UNDECLARED, ARRAY} GRAMTYPE;

const char *printGRAM(int x);
const char *printINPUT(int x);


class Lexeme
        {
        public:
        GRAMTYPE type; 
        string String; 
        int value; 
        bool boolean;
        Lexeme *left;
        Lexeme *right;
        Lexeme *carValue;
        Lexeme *cdrValue;
        vector<Lexeme*> array;

        Lexeme(GRAMTYPE t);
        Lexeme(GRAMTYPE t, string str);
        Lexeme(GRAMTYPE t, string str, int value);
        Lexeme(GRAMTYPE t, string str, bool b);
        void display(void);
        };

class Lexer
    {
    public:
    ifstream Input;

    Lexer();
    Lexer(char *);

    void skipWhiteSpace(char ch);
    Lexeme* lexVariableOrKeyword(char ch);
    Lexeme* lexNumber(char ch);
    Lexeme* lexString(char ch);
    Lexeme* lexComment(char ch);
    Lexeme* lex(void);

    private:
    };

void scanner(char* filename);

#endif