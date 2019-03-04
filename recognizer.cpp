#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include "lexer.h"
#include "env.h"
#include "evaluate.h"
using namespace std;

class Recognizer 
	{
	public:
	Lexeme *CurrentLexeme;
	Lexer *myLexer;

	Recognizer(Lexeme *curr, Lexer *l) 
		{
		CurrentLexeme = curr;
		myLexer = l;
		};

	void printError(GRAMTYPE type)
		{
		cerr << "Syntax error -  Type is: ";
		cerr << printGRAM((*CurrentLexeme).type);
		cerr << " but expected type was: ";
		cerr << printGRAM(type) << endl;
		exit(1);
		}

	bool check(GRAMTYPE type)
		{
		if ((*CurrentLexeme).type == COMMENT) advance();
		return ((*CurrentLexeme).type == type);  //comparing the two types
		}

	Lexeme *advance(void) 
	    {
    	Lexeme *temp = CurrentLexeme;
	    CurrentLexeme = (*myLexer).lex();
	    return temp; 
	    } 
	  
	Lexeme *match(GRAMTYPE type) 
	    { 
	    matchNoAdvance(type); 
	    return advance(); 
	    }

	void matchNoAdvance(GRAMTYPE type)
	    {
	    if (!check(type))
	        printError(type);
	    }

	Lexeme *program(void)
		{
		Lexeme *tree = new Lexeme(PROGRAM);
		(*tree).left = programExpressionList();

		//right recursion
		if (programPending())
			(*tree).right = program();
		return tree;
		}

	Lexeme *programExpressionList(void)
		{
		Lexeme *tree;

		if (functionDefPending())
			tree = functionDef();
		else if (varDefPending())
			tree = varDef();
		else if (functionCallPending())
			tree = functionCall();
		else
			{
			cerr << "Syntax error: a functionDef, functionCall, or varDef was not provided." << endl;
			exit(1);
			}

		return tree;
		}

	Lexeme *expression(void)
	    {
	    Lexeme *u = unary();

	    if (opPending()) //a binary expression
	        {
	        Lexeme *tree = new Lexeme(BINARY); //this is the tag
	        (*tree).left = op();
	        (*tree).right = new Lexeme(GLUE); //an interior node
	        (*(*tree).right).left = u;
	        (*(*tree).right).right = expression();
	        return tree;
	        }
	    else //a unary expression
	        return u;
	    }

	Lexeme *op(void)
	    {
	    Lexeme *tree;
	    //the tree is the operator that was found
	    if (check(PLUS)) tree = advance();
	    else if (check(MINUS)) tree = advance();
	    else if (check(TIMES)) tree = advance();
	    else if (check(DIVIDES)) tree = advance();
	    else if (check(ASSIGN)) tree = advance();
	    else if (logicalOpPending()) tree = logicalOp();
	    else 
	    	{
	    	tree = NULL; //shouldn't happen, operator not found
	    	printError(OPERATOR);
	    }

	    return tree;
	    }

	Lexeme *logicalOp(void)
	    {
	    Lexeme *tree;
	    
	    if (check(EQUAL)) tree = advance();
	    else if (check(LESSTHAN)) tree = advance();
	    else if (check(GREATERTHAN)) tree = advance();
	    else if (check(AND)) tree = advance();
	    else if (check(OR)) tree = advance();
	    else 
	    	{
	    	tree = NULL; //shouldn't happen, operator not found
	    	printError(LOGICALOPERATOR);
	    	}

	    return tree;
	    }

	/*Lexeme *expressionList(void)
		{
		Lexeme *tree;

		if (functionDefPending())
			tree = functionDef();
		else if (varDefPending())
			tree = varDef();
		else if (functionCallPending())
			tree = functionCall();
		else
			{
			cerr << "Syntax error." << endl;
			exit(1);
			}

		return tree;
		}*/

	Lexeme *optExpressionList(void)
		{
		if (expressionListPending()) return expressionList();
		else 
			return NULL;
		}

	////////////// first set functions ///////////////

	bool programPending(void)
		{
		return  functionDefPending() || varDefPending() || functionCallPending();
		}

	bool opPending(void)
	    {
	    return check(PLUS) || check(MINUS) || check(TIMES) || check(DIVIDES) || check(ASSIGN) || logicalOpPending();
	    }

	bool logicalOpPending(void)
		{
		return check(EQUAL) || check(LESSTHAN) || check(GREATERTHAN) || check(AND) || check(OR);
		}

	bool expressionListPending(void)
	    {
	    return expressionPending() || functionDefPending();
	    }

	bool expressionPending(void)
	    {
	    return unaryPending();
	    }

	bool unaryPending(void)
	    {
	    return check(INTEGER) || check(VARIABLE) || check(STRING) || check(OPAREN) || check(MINUS) || check(BOOLEAN) || check(NOT) || check(NIL) || check(ARRAY);
	    }

	bool ifStatementPending(void)
		{
		return check(IF);
		}

	bool blockPending(void)
		{
		return check(OBRACE);
		}

	bool statementPending(void)
		{
		return expressionPending() || varDefPending() ||functionCallPending() || functionDefPending()|| ifStatementPending() || whileLoopPending() || forLoopPending();
		}

	bool statementsPending(void)
		{
		return statementPending();
		}

	bool whileLoopPending(void)
		{
		return check(WHILE);
		}

	bool forLoopPending(void)
		{
		return check(FOR);
		}

	bool varDefPending(void)
		{
		return check(VAR);
		}

	bool functionDefPending(void)
		{
		return check(FUNCTION) || check(ANONYMOUS);
		}

	bool functionCallPending(void)
		{
		return check(VARIABLE);
		}

	bool parameterListPending(void)
		{
		return check(VARIABLE);
		}

	Lexeme *unary(void) 
	    {
	    Lexeme *tree;

	    if (check(INTEGER)) tree = advance();
	    else if (check(STRING)) tree = advance();
	    else if (check(NIL)) tree = advance();
	    else if (check(INTEGER)) tree = advance();
	    else if (check(BOOLEAN)) tree = advance();
	    else if (check(ARRAY)) tree = advance();
	    else if (check(VARIABLE)) 
	    	{
	    	Lexeme *temp = match(VARIABLE);

	    	if (check(OPAREN)) 
	    		{
	    		tree = new Lexeme(FUNCTIONCALL);
	    		(*tree).left = temp;
	    		advance();
	    		(*tree).right = optExpressionList();
	    		match(CPAREN); //advance matches OPAREN()
	    		}

	    	else tree = temp;
	    	}
	    else if (check(OPAREN))
	        {
	        //tree = match(OPAREN); //tree is tagged OPAREN
	        tree = advance();
	        (*tree).left = NULL;
	        (*tree).right = expression();
	        match(CPAREN);
	        }
	    else if (check(MINUS))
	        {
	        //match(MINUS);
	        advance();
	        tree = new Lexeme(UMINUS); //tag the tree
	        (*tree).left = NULL;
	        (*tree).right = unary();
	        }
	    else if (check(NOT))
	    	{
	    	//match(NOT);
	    	advance();
	    	tree = new Lexeme(NOT);
	    	(*tree).left = NULL;
	    	(*tree).right = unary();
	    	}
	    else if (functionDefPending()) tree = functionDef();
	    else
	    	{
	    	tree = NULL;
	    	printError(UNARY);
	    	return NULL;
	    	}

	    return tree;
	    }

	Lexeme *expressionList(void)
	    {
	    Lexeme *temp;

	    if (expressionPending())
	    		{
				temp = expression();
			    if (check(COMMA))
			        {
			        Lexeme *tree = new Lexeme(LIST);
			        (*tree).left = temp;
			        //match(COMMA);
			        advance();
			        (*tree).right = expressionList();
			        return tree;
	        		}
	        	else 
	        		return temp;	
	    		}

	    else if (check(ANONYMOUS))
	    		{
	    		temp = functionDef();
	    		if (check(COMMA))
			        {
			        Lexeme *tree = new Lexeme(LIST);
			        (*tree).left = temp;
			        //match(COMMA);
			        advance();
			        (*tree).right = expressionList();
			        return tree;
	        		}
	        	else 
	        		return temp;
	    		}

		return NULL;
	    }

	Lexeme *ifStatement(void)
		{
		Lexeme *tree;
		tree = match(IF);
		match(OPAREN);
		(*tree).left = expression();
		match(CPAREN);
		(*tree).right = new Lexeme(GLUE);
		(*(*tree).right).left = block();
		(*(*tree).right).right = optElse();
		return tree;
		}

	Lexeme *optElse(void)
		{
		Lexeme *tree;

		if (check(ELSE))
				{
				tree = advance();

				if (blockPending())
						{
						(*tree).left = NULL;
						(*tree).right = block();
						return tree;
						}
				else if (ifStatementPending())
						{
						(*tree).left = NULL;
						(*tree).right = ifStatement();
						return tree;
						}
				else
						{
						cerr << "Syntax error - Opt else." << endl;
						exit(1);
						}
				}
		else 
			return NULL;
		}
	
	Lexeme *block(void)
		{
		Lexeme *tree;
		tree = match(OBRACE);
		(*tree).left = NULL;
		(*tree).right = statements();
		match(CBRACE);
		return tree;
		}

	Lexeme *statement(void)
		{
		Lexeme *tree;

		if (expressionPending()) tree = expression();
		else if (varDefPending()) tree = varDef();
		else if (functionCallPending()) tree = functionCall();
		else if (functionDefPending()) tree = functionDef();
		else if (ifStatementPending()) tree = ifStatement();
		else if (whileLoopPending()) tree = whileLoop();
		else if (forLoopPending()) tree = forLoop();		
		else
			{
			tree = NULL;
			printError(STATEMENT);
			}

		return tree;
		}

	Lexeme *statements(void)
		{
		Lexeme *temp;
		Lexeme *tree;
		temp = statement();

		if (statementsPending())
			{
			tree = new Lexeme(STATEMENTS);
			(*tree).left = temp;
			(*tree).right = statements();
			return tree;
			}

		else 
			return temp;
		}

	Lexeme *whileLoop(void)
		{
		Lexeme *tree;
		tree = match(WHILE);
		match(OPAREN);
		(*tree).left = expression();
		match(CPAREN);
		(*tree).right = block();

		return tree;
		}

	Lexeme *forLoop(void)
		{
		Lexeme *tree;
		tree = match(FOR);
		match(OPAREN);
		(*tree).left = new Lexeme(GLUE);
		(*(*tree).left).left = match(VARIABLE);
		(*(*tree).left).right = new Lexeme(GLUE);
		(*(*(*tree).left).right).left = expression();
		(*(*(*tree).left).right).right = expression();
		match(CPAREN);
		(*tree).right = block();

		return tree;
		}

	Lexeme *functionDef(void)
		{
		Lexeme *tree;

		if (check(ANONYMOUS))
				{
				tree = match(ANONYMOUS);
				match(OPAREN);
				Lexeme *rightleft = optParameterList();
				match(CPAREN);
				Lexeme *rightright = block();

				if (check(OBRACKET))
						{
						tree = new Lexeme(ACALL);
						advance();
						(*tree).left = optExpressionList();
						(*tree).right = new Lexeme(GLUE);
						(*(*tree).right).left = rightleft;
						(*(*tree).right).right = rightright;
						match(CBRACKET);
						}
				else
						{
						(*tree).left = NULL;
						(*tree).right = new Lexeme(GLUE);
						(*(*tree).right).left = rightleft;
						(*(*tree).right).right = rightright;
						}
				return tree;
				}
		tree = match(FUNCTION);
		(*tree).left = match(VARIABLE);
		match(OPAREN);
		(*tree).right = new Lexeme(GLUE);
		(*(*tree).right).left = optParameterList();
		match(CPAREN);
		(*(*tree).right).right = block();

		return tree;
		}

	Lexeme *functionCall(void)
		{
		Lexeme *tree = new Lexeme(FUNCTIONCALL);
		(*tree).left = match(VARIABLE);
		match(OPAREN);
		(*tree).right = optExpressionList();
		match(CPAREN);

		return tree;
		}
				
	Lexeme *varDef(void)
		{
		Lexeme *tree;
		tree = match(VAR);
		(*tree).left = match(VARIABLE);
		(*tree).right = NULL;

		if (check(ASSIGN))
				{
				advance();

				if (expressionPending())
						{								
						(*tree).right = expression();
						}
				else if (functionDefPending())
						{
						(*tree).right = expression();
						}
				else
						{
						cerr << "Syntax error - assignment." << endl;
						exit(1);
						}
				}

		return tree;
		}

	Lexeme *parameterList(void)
		{
		Lexeme *root = new Lexeme(OPAREN);
		(*root).left = NULL;			
		(*root).right = paramHelp();

		return root;
		}

	Lexeme *paramHelp(void)
		{
		Lexeme *temp = match(VARIABLE);
		Lexeme *tree = new Lexeme(LIST);

		if (check(COMMA))
			{
			advance();
			(*tree).left = temp;
			(*tree).right = paramHelp();
			return tree;
			}
		return temp;
		}

	Lexeme *optParameterList(void)
        {
        if (parameterListPending())
            return parameterList();
        else
            return NULL; //NULL signifies an empty list
        }

    void prettyPrint(Lexeme *tree)
	    {
	    if (tree == NULL) return;

	    switch ((*tree).type)
	        {
	        case INTEGER:  { cout << (*tree).value; break; }
	        case VARIABLE: { cout << (*tree).String; break; }
	        case STRING:   { cout << "\"" << (*tree).String << "\""; break; }
	        case BOOLEAN:  { cout << (*tree).String; break; }
	        case NIL: {cout << "nil"; break; }
	        case OPAREN:
	            {
	            cout << "(";
	            prettyPrint((*tree).right);
	            cout << ")";
	            break;
	            }
	        case UMINUS:
	            {
	            cout << "-";
	            prettyPrint((*tree).right);
	            break;
	            }
	        case BINARY:
	            {
	            prettyPrint((*(*tree).right).left);  //the left operand
	            cout << printINPUT((*(*tree).left).type);              //the operator
	            prettyPrint((*(*tree).right).right); //the right operand
	            break;
	            }
	        case FUNCTION:
			    {
			    cout << "function ";
			    prettyPrint((*tree).left);
			    if ((*(*tree).right).left == NULL) cout << "( )";
			    prettyPrint((*tree).right);
			    cout << endl;
			    break;
			    }
			case FUNCTIONCALL:
				{
				prettyPrint((*tree).left);
				cout << "(";
				prettyPrint((*tree).right);
				cout << ") ";
				break;
				}
			case STATEMENTS:
				{
				cout << " ";
				prettyPrint((*tree).left);
				prettyPrint((*tree).right);
				break;
				}
			case LIST:
				{
				prettyPrint((*tree).left);
				if ((*tree).right != NULL)
					{
					cout <<", ";
					prettyPrint((*tree).right);	
					}
				break;
				}
			case NOT:
				{
				cout << "!";
				prettyPrint((*tree).right);
				break;
				}
			case IF:
				{
				cout << "\nif (";
				prettyPrint((*tree).left);
				cout << ") \n\t";
				prettyPrint((*tree).right);
				break;
				}
			case ELSE:
				{
				cout << "else ";
				prettyPrint((*tree).right);
				break;
				}
			case WHILE:
				{
				cout << "while (";
				prettyPrint((*tree).left);
				cout << ") ";
				prettyPrint((*tree).right);
				break;
				}
			case FOR:
				{
				cout << "for (";
				prettyPrint((*tree).left);
				cout << ") ";
				prettyPrint((*tree).right);
				break;
				}
			case ANONYMOUS:
				{
				cout << "anonymous ";
				prettyPrint((*(*tree).right).left);
				prettyPrint((*(*tree).right).right);
				
				if ((*tree).left != NULL)
					{
					cout << "(";
					prettyPrint((*tree).left);
					cout << ")";	
					}
				else 
					prettyPrint((*tree).left);

				break;
				}
			case ACALL:
				{
				cout << "anonymous ";
				prettyPrint((*(*tree).right).left);
				prettyPrint((*(*tree).right).right);
				cout << "[";
				prettyPrint((*tree).left);
				cout << "]";
				break;
				}
			case OBRACE:
				{
				cout << "{ ";
				prettyPrint((*tree).right);
				cout << "} ";
				break;
				}
			case GLUE:
				{
				prettyPrint((*tree).left);
				prettyPrint((*tree).right);
				break;
				}
			case VAR:
				{
				cout << "var ";
				prettyPrint((*tree).left);
				if ((*tree).right != NULL)
					{
					cout << " = ";
					prettyPrint((*tree).right);
					}	
				cout << " ";
				break;
				}
			case PROGRAM:
				{
				prettyPrint((*tree).left);
				prettyPrint((*tree).right);
				break;
				}

			//case COMMENT:
	        //...
	        default: 
	        	{ 
	        	cerr << "Bad expression!" << endl;
	        	exit(1);
	        	}
	    	}
	    }

	Lexeme *recognize(bool b)
		{
		Lexeme *tree;
		advance();
		tree = program();
		advance();

		if ((*CurrentLexeme).type != ENDofINPUT)
			printError(ENDofINPUT);

		if (b == true)
			{
			cout << "Pretty printing" << endl;
			prettyPrint(tree);
			cout << endl << "Finished pretty printing" << endl;
			}

		return tree;
		}

	
	private:	
	};

//main function of DPL
int main(int argc, char* argv[])
    {
    Lexeme *token = new Lexeme(UNKNOWN);
    char *input = argv[1];
    Lexer *l = new Lexer(input);
    Recognizer *r = new Recognizer(token, l);
    Lexeme *tree = (*r).recognize(true);
    cout << endl;
    Lexeme *env = create();
    eval(tree, env);

    return 0;
    }

