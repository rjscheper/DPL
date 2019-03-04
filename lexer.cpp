#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;

//grammar types for the Lexemes
typedef enum gramtypes {IF, AND, OR, WHILE, FOR, ELSE, EQUAL, VARIABLE, INTEGER, STRING, PROGRAM, 
    FUNCTION, VAR, ENDofINPUT, COMMENT, LIST, FUNCTIONCALL, STATEMENT, STATEMENTS, BOOLEAN, OPAREN, CPAREN, COMMA, SEMICOLON, 
    UNKNOWN, ASSIGN, PLUS, TIMES, MINUS, DIVIDES, LESSTHAN, GREATERTHAN, OPERATOR, LOGICALOPERATOR, UNARY, DOT, GLUE, BINARY, 
    NOT, NEWLINE, NIL, UMINUS, ANONYMOUS, ACALL, ENV, OBRACE, CBRACE, OBRACKET, CBRACKET, TABLE, CLOSURE, UNDECLARED, ARRAY} GRAMTYPE;

const char *printGRAM(int x) {
    const char *printGRAMTYPES[] = {
    "IF",
    "AND",
    "OR",
    "WHILE",
    "FOR",
    "ELSE",
    "EQUAL",
    "VARIABLE",
    "INTEGER",
    "STRING",
    "PROGRAM",
    "FUNCTION",
    "VAR",
    "ENDofINPUT",
    "COMMENT",
    "LIST",
    "FUNCTIONCALL",
    "STATEMENT",
    "STATEMENTS",
    "BOOLEAN",
    "OPAREN",
    "CPAREN",
    "COMMA",
    "ASSIGN",
    "SEMICOLON",
    "UNKNOWN",
    "PLUS",
    "TIMES",
    "MINUS",
    "DIVIDES",
    "LESSTHAN",
    "GREATERTHAN",
    "OPERATOR",
    "LOGICALOPERATOR",
    "UNARY",
    "DOT",
    "GLUE",
    "BINARY",
    "NOT",
    "NEWLINE",
    "NIL",
    "UMINUS",
    "ANONYMOUS",
    "ACALL",
    "ENV",
    "OBRACE",
    "CBRACE",
    "OBRACKET",
    "CBRACKET",
    "TABLE",
    "CLOSURE",
    "UNDECLARED",
    "ARRAY"
    
    };
    return printGRAMTYPES[x];
}

const char *printINPUT(int x) {
    const char *printGRAMTYPES[] = {
    "if",
    "and",
    "or",
    "while",
    "for",
    "else",
    "equal",
    "VARIABLE",
    "INTEGER",
    "STRING",
    "program",
    "function",
    "var",
    "ENDofINPUT",
    "@",
    "LIST",
    "FUNCTIONCALL",
    "STATEMENT",
    "STATEMENTS",
    "BOOLEAN"
    "OPAREN",
    "CPAREN",
    ",",
    " = "
    ";",
    "UNKNOWN",
    " + ",
    " * ",
    " - ",
    " / ",
    " < ",
    " > ",
    "OPERATOR",
    "LOGICALOPERATOR",
    "UNARY",
    "GLUE",
    "BINARY",
    "!",
    " . ",
    "NEWLINE",
    "NIL",
    " - ",
    "ANONYMOUS",
    "ACALL",
    "ENV",
    "{",
    "}",
    "[",
    "]",
    "TABLE",
    "CLOSURE",
    "UNDECLARED",
    "ARRAY"
    
    };
    return printGRAMTYPES[x];
}

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

    Lexeme(GRAMTYPE t)
        {
        type = t;
        String = "";
        value = 0;
        boolean = true;
        left = NULL;
        right = NULL;
        carValue = NULL;
        cdrValue = NULL;
        };

    Lexeme(GRAMTYPE t, string str)
        {
        type = t;
        String = str;
        value = 0;
        boolean = false;
        left = NULL;
        right = NULL;
        carValue = NULL;
        cdrValue = NULL;
        };

    Lexeme(GRAMTYPE t, string str, int val)
        {
        type = t;
        String = str;
        value = val;
        boolean = false;
        left = NULL;
        right = NULL;
        carValue = NULL;
        cdrValue = NULL;
        };

    Lexeme(GRAMTYPE t, string str, bool b)
        {
        type = t;
        String = str;
        value = 0;
        boolean = b;
        left = NULL;
        right = NULL;
        carValue = NULL;
        cdrValue = NULL;
        };

    //used in scanner
    void display(void)
        {
        cout << printGRAM(type);

        if (!(String.empty())) {
            cout << ": " << value;
        }

        if (type == INTEGER) {
            cout << ": " << value;
        }
        
        cout << endl;
        }

    private:
    }; 


class Lexer
    {
    public:
    ifstream Input;

    Lexer();

    Lexer(char *IN) 
        {
        Input.open(IN);
        };

    void skipWhiteSpace(char ch)
        {
        if (isspace(ch))
            Input >> ch;

        Input.putback(ch);      //using C++ putback as opposed to pushback
        }

    Lexeme* lexVariableOrKeyword(char ch)
        {
        string token = "";
        
        while (isalpha(ch) || isdigit(ch))
            {
            token = token + ch;
            Input.get(ch);
            }

        //push back the character that got us out of the loop
        //it may be some kind of punctuation

        Input.putback(ch);

        //token holds either a variable or a keyword, so figure it out

        if (token.compare("if") == 0) return new Lexeme(IF);
        else if (token.compare("and") == 0) return new Lexeme(AND);
        else if (token.compare("or") == 0) return new Lexeme(OR);
        else if (token.compare("while") == 0) return new Lexeme(WHILE);
        else if (token.compare("else") == 0) return new Lexeme(ELSE);
        else if (token.compare("for") == 0) return new Lexeme(FOR);
        else if (token.compare("equal") == 0) return new Lexeme(EQUAL);
        else if (token.compare("program") == 0) return new Lexeme(PROGRAM);
        else if (token.compare("function") == 0) return new Lexeme(FUNCTION);
        else if (token.compare("anonymous") == 0) return new Lexeme(ANONYMOUS);
        else if (token.compare("var") == 0) return new Lexeme(VAR);
        else if (token.compare("and") == 0) return new Lexeme(AND);
        else if (token.compare("nLine") == 0) return new Lexeme(NEWLINE);
        else if (token.compare("nil") == 0) return new Lexeme(NIL);
        else if (token.compare("true") == 0) return new Lexeme(BOOLEAN, "true", true);
        else if (token.compare("false") == 0) return new Lexeme(BOOLEAN, "false", false);

        //more keyword testing here
        else //must be a variable!
            return new Lexeme(VARIABLE,token);
        }

    Lexeme* lexNumber(char ch)
        {
        string num;

        while (isdigit(ch))
            {
            num = num + ch;
            Input.get(ch);
            }

        Input.putback(ch);

        int i = atoi(num.c_str());
        Lexeme *result = new Lexeme(INTEGER, num);
        (*result).value = i;
        return result;
        }

    Lexeme* lexString(char ch)
        {
        string str;
        Input.get(ch);

        while (ch != '\"')
            {
            str = str + ch;
            Input.get(ch);
            }

        Lexeme *result = new Lexeme(STRING, str);
        return result;
        }

    Lexeme* lexComment(char ch)
        {
        string comment = "";
        Input.get(ch);

        while (ch != '@')
            {
            comment = comment + ch;
            Input.get(ch);
            }

        Lexeme *result = new Lexeme(COMMENT, comment);
        return result;
        }

    Lexeme* lex() 
        { 
        char ch;

        Input.get(ch);

        skipWhiteSpace(ch); 

        if (!(Input >> ch)) return new Lexeme(ENDofINPUT); 

        switch(ch) 
            { 
            // single character tokens 

            case '(': return new Lexeme(OPAREN); 
            case ')': return new Lexeme(CPAREN); 
            case ',': return new Lexeme(COMMA); 
            case '+': return new Lexeme(PLUS);
            case '*': return new Lexeme(TIMES); 
            case '-': return new Lexeme(MINUS); 
            case '/': return new Lexeme(DIVIDES); 
            case '<': return new Lexeme(LESSTHAN); 
            case '>': return new Lexeme(GREATERTHAN); 
            case '=': return new Lexeme(ASSIGN); 
            case ';': return new Lexeme(SEMICOLON);
            case '{': return new Lexeme(OBRACE);
            case '}': return new Lexeme(CBRACE);
            case '[': return new Lexeme(OBRACKET);
            case ']': return new Lexeme(CBRACKET);
            case '.': return new Lexeme(DOT);
            case '!': return new Lexeme(NOT);

            //add any other cases here

            default: 
                // multi-character tokens (only numbers, 
                // variables/keywords, and strings) 
                if (isdigit(ch)) 
                    { 
                    return lexNumber(ch); 
                    } 
                else if (isalpha(ch)) 
                    { 
                    return lexVariableOrKeyword(ch);
                    } 
                else if (ch == '\"') 
                    { 
                    return lexString(ch); 
                    }
                else if (ch == '@') 
                    { 
                    return lexComment(ch); 
                    }
                else
                    return new Lexeme(UNKNOWN, "UNKNOWN"); 
            } 
        }

    private:
    };

//not used
void scanner(char* filename) 
    { 
    Lexeme *token; 
    Lexer *i = new Lexer(filename);

    token = (*i).lex(); 
    while ((*token).type != ENDofINPUT)
        { 
        (*token).display(); 
        //cout << "\n"
        token = (*i).lex(); 
        } 
    }

//for testing
/*int main(int argc, char* argv[])
    {
        scanner(argv[1]);
        return 0;
    }*/