#include "evaluate.h"
using namespace std;

Lexeme *eval(Lexeme *tree, Lexeme *env)
    {
    if (tree == NULL) return NULL;

    switch ((*tree).type)
        {
        //self evaluating
        case INTEGER: return tree;
        case STRING: return tree;
        case BOOLEAN: return tree;
        case NIL: return tree;
        //find value of variables in the environment
        case VARIABLE: return lookupEnv(tree,env);
        //parenthesized expression
        case OPAREN: return eval((*tree).right,env);
        case BINARY: return evalSimpleOp(tree, env);
        //operators (both sides evaluated)
        //case PLUS:
        //case MINUS:
        //case TIMES:
        //case DIVIDES:

        case GREATERTHAN: return evalSimpleOp(tree,env);
        //AND and OR short-circuit
        case AND:
        //case OR: return evalShortCircuitOp(tree,env);
        //dot operator evals lhs, rhs a variable
        //case DOT: return evalDot(tree,env);
        //assign operator evals rhs for sure
        //    lhs is a variable or a dot operation
        //case ASSIGN: return evalAssign(tree,env);

        //variable and Lexeme definitions
        case VAR: { evalVarDef(tree,env); return NULL; }
        case FUNCTION: { evalFuncDef(tree,env); return NULL; }
        case ANONYMOUS: { return evalAnonDef(tree,env); }
        case ACALL: { return evalAnonCall(tree,env); }
        //imperative constructs
        case IF: return evalIf(tree,env);
        case WHILE: return evalWhile(tree,env);
        case ELSE: return eval((*tree).right,env); //left is null -> evaluate block/if
        //Lexeme calls
        case FUNCTIONCALL: 
        	{
        	Lexeme *result = evalCall(tree,env);
        	return eval(result,env);
        	}
        case PROGRAM:
        	{
        	Lexeme *result = evalProgram(tree,env);
        	return result;
        	}
        case GLUE: return evalGlue(tree,env);
        case STATEMENTS: return evalGlue(tree,env);
        case UMINUS: return evalUMinus(tree,env);
        case NOT: return evalNot(tree,env);
        case ARRAY: return tree;
        //program and Lexeme body are parsed as blocks
        case OBRACE: return evalBlock(tree,env);

        default: { cerr << "bad expression!"; exit(1); }
        }
    return NULL;
    }

void evalFuncDef(Lexeme *tree, Lexeme *env)
    {
    Lexeme *closure = cons(CLOSURE,env,tree);
    insertEnv(getFuncName(tree),closure,env);
    }

void evalVarDef(Lexeme *tree, Lexeme *env)
    {
    //error check
    if ((*(*tree).left).type != VARIABLE)
        {
        cerr << "Syntax error: Must assign with variable." << endl;
        exit(-1);
        }
    
    //eval the right hand side
    Lexeme *value = eval((*tree).right,env);
    insertEnv((*tree).left, value, env);
    }

Lexeme *evalArgs(Lexeme *args, Lexeme *env)
    {
    Lexeme *result = args;

    while (args != NULL)
        {
        setCar(args, eval(car(args), env));
        args = cdr(args);
        }

    return result;
    }

Lexeme *apList(Lexeme *tree, bool b)
    {
    if (tree == NULL) return NULL;

    if ((*tree).type == OPAREN || (*tree).type == OBRACE)
        return apList((*tree).right, true);

    else if ((*tree).type == GLUE|| (*tree).type == LIST)
        return cons(GLUE, (*tree).left, apList((*tree).right, false));

    else if ((*tree).type == ANONYMOUS)
        return cons(GLUE, tree, NULL);

    else
        {
        if (b == true)
            return cons(GLUE, tree, apList((*tree).right, false));
        else
            return cons(GLUE, tree, NULL);
        }
    return NULL;
    }

Lexeme *evalAnonDef(Lexeme *tree, Lexeme *env)
    {
    return cons(CLOSURE, env, tree);
    }

Lexeme *evalAnonCall(Lexeme *tree, Lexeme *env)
    {
    Lexeme *args = (*tree).left;
    args = apList(args, true);
    Lexeme *eargs = evalArgs(args,env);
    Lexeme *params = (*(*tree).right).left;
    params = apList(params, true);
    Lexeme *body = (*(*tree).right).right;
    Lexeme *xenv = extendEnv(env,params,eargs);
    return eval(body, xenv);
    }

Lexeme *evalProgram(Lexeme *tree, Lexeme *env)
    {
    eval((*tree).left, env);
    Lexeme *result = eval((*tree).right, env);
    return result;
    }

Lexeme *getFuncName(Lexeme *tree)
    {
    if ((*(*tree).left).type != VARIABLE)
        {
        cerr << "Error: invalid function name." << endl;
        exit(1);
        }

    else
        return (*tree).left;
        
    }

Lexeme *getFuncCallArgs(Lexeme *tree)
    {
    return (*tree).right;
    }

Lexeme *getClosureParams(Lexeme *closure)
    {
    Lexeme *paramTree = (*cdr(closure)).right;
    return (*paramTree).left;
    }

Lexeme *getClosureBody(Lexeme *closure)
    {
    Lexeme *bodyTree = (*cdr(closure)).right;
    return (*bodyTree).right;
    }

Lexeme *getClosureEnvironment(Lexeme *closure)
    {
    return car(closure);
    }

Lexeme *evalCall(Lexeme *tree, Lexeme *env) //
    {
    Lexeme *funcDefName = getFuncName(tree);
    Lexeme *args = getFuncCallArgs(tree);
    args = apList(args, true);
    Lexeme *eargs = evalArgs(args,env);

    //check for built-in functions here
    if ((*funcDefName).String.compare("print") == 0)
        {
        evalPrint(eargs);
        return NULL;
        }
    if ((*funcDefName).String.compare("isInt") == 0)
        return isInt(eargs);
    if ((*funcDefName).String.compare("makeArray") == 0)
        return evalMakeArray(eargs);
    if ((*funcDefName).String.compare("setArray") == 0) 
        {
        evalSetArray(eargs);
        return NULL;
        }
    if ((*funcDefName).String.compare("getArray") == 0)
        return evalGetArray(eargs);
    if ((*funcDefName).String.compare("showEnv") == 0)
        {
        displayEnv(eargs);
        return NULL;
        }
    
    Lexeme *closure = eval(funcDefName,env);
    Lexeme *params = getClosureParams(closure);
    params = apList(params, true);
    Lexeme *body = getClosureBody(closure);
    Lexeme *senv = getClosureEnvironment(closure);
    Lexeme *xenv = extendEnv(senv,params,eargs);
    return eval(body,xenv);
    }

Lexeme *evalBlock(Lexeme *tree, Lexeme *env)
    {
    Lexeme *result;
    result = eval((*tree).right, env);
    /*
    while (tree != null)
        {
        result = eval((*tree).left, env);
        tree = tree . right;
        }
    */
    return result;
    }

Lexeme *evalSimpleOp(Lexeme *tree, Lexeme *env)
    {
    if ((*(*tree).left).type == PLUS) return evalPlus((*tree).right,env);
    if ((*(*tree).left).type == MINUS) return evalMinus((*tree).right,env);
    if ((*(*tree).left).type == TIMES) return evalTimes((*tree).right,env);
    if ((*(*tree).left).type == DIVIDES) return evalDivides((*tree).right,env);
    if ((*(*tree).left).type == EQUAL) return evalEqual((*tree).right,env);
    if ((*(*tree).left).type == AND) return evalAnd((*tree).right,env);
    if ((*(*tree).left).type == OR) return evalOr((*tree).right,env);
    if ((*(*tree).left).type == LESSTHAN) return evalLessThan((*tree).right,env);
    if ((*(*tree).left).type == GREATERTHAN) return evalGreaterThan((*tree).right,env);
    if ((*(*tree).left).type == ASSIGN) 
    	{
    	evalAssign((*tree).right,env);
    	return NULL;
    	}

    else 
    	return NULL;
    }

Lexeme *newIntegerLexeme(int val)
    {
    Lexeme *result = new Lexeme(INTEGER);
    (*result).value = val;
    return result;
    }

Lexeme *newStringLexeme(string s)
    {
    Lexeme *result = new Lexeme(STRING);
    (*result).String = s;
    return result;
    }

Lexeme *newBooleanLexeme(bool b)
    {
    Lexeme *result = new Lexeme(STRING);
    (*result).boolean = b;
    return result;
    }

Lexeme *evalPlus(Lexeme *tree, Lexeme *env)
    {
    //eval the left and the right hand sides
    Lexeme *left = eval((*tree).left,env);
    Lexeme *right = eval((*tree).right,env);
    if ((*left).type == INTEGER && (*right).type == INTEGER)
        return newIntegerLexeme((*left).value + (*right).value);
    else if ((*left).type == STRING && (*right).type == STRING)
        return newStringLexeme((*left).String + (*right).String);
    else if ((*left).type == BOOLEAN && (*right).type == BOOLEAN)
        return newBooleanLexeme(true);
    else
    	return left;
    }

Lexeme *evalMinus(Lexeme *tree, Lexeme *env)
    {
    //eval the left and the right hand sides
    Lexeme *left = eval((*tree).left,env);
    Lexeme *right = eval((*tree).right,env);
    if ((*left).type == INTEGER && (*right).type == INTEGER)
        return newIntegerLexeme((*left).value - (*right).value);
    else if ((*left).type == STRING && (*right).type == STRING)
        return newStringLexeme((*left).String);
    else if ((*left).type == BOOLEAN && (*right).type == BOOLEAN)
        return newBooleanLexeme(false);
    else
        return left;
    }

Lexeme *evalUMinus(Lexeme *tree, Lexeme *env)
    {
    return newIntegerLexeme(0 - (*(*tree).right).value);
    }

Lexeme *evalTimes(Lexeme *tree, Lexeme *env)
    {
    //eval the left and the right hand sides
    Lexeme *left = eval((*tree).left,env);
    Lexeme *right = eval((*tree).right,env);
    if ((*left).type == INTEGER && (*right).type == INTEGER)
        return newIntegerLexeme((*left).value * (*right).value);
    else if ((*left).type == STRING && (*right).type == STRING)
        return newStringLexeme((*left).String);
    else if ((*left).type == BOOLEAN && (*right).type == BOOLEAN)
        return newBooleanLexeme((*left).boolean);
    else
        return left;
    }

Lexeme *evalDivides(Lexeme *tree, Lexeme *env)
    {
    //eval the left and the right hand sides
    Lexeme *left = eval((*tree).left,env);
    Lexeme *right = eval((*tree).right,env);
    if ((*left).type == INTEGER && (*right).type == INTEGER)
        return newIntegerLexeme((*left).value / (*right).value);
    else if ((*left).type == STRING && (*right).type == STRING)
        return newStringLexeme((*left).String);
    else if ((*left).type == BOOLEAN && (*right).type == BOOLEAN)
        return newBooleanLexeme((*left).boolean);
    else
        return left;
    }

Lexeme *evalEqual(Lexeme *tree, Lexeme *env)
    {
    //eval the left and the right hand sides
    Lexeme *left = eval((*tree).left,env);
    Lexeme *right = eval((*tree).right,env);

    if ((*left).type == INTEGER && (*right).type == INTEGER)
        return newBooleanLexeme((*left).value == (*right).value);
    else if ((*left).type == STRING && (*right).type == STRING)
        return newBooleanLexeme((*left).String.compare((*right).String) == 0);
    else if ((*left).type == BOOLEAN && (*right).type == BOOLEAN)
        return newBooleanLexeme((*left).boolean == (*right).boolean);
    else if ((*left).type == NIL && (*right).type == NIL)
        return newBooleanLexeme(true);
    else
        return newBooleanLexeme(left == right);
    }

Lexeme *evalNot(Lexeme *tree, Lexeme *env)
    {
    Lexeme *b = eval((*tree).right, env);
    if ((*b).boolean == true)
        return newBooleanLexeme(false);
    else
        return newBooleanLexeme(true);
    }

Lexeme *evalGlue(Lexeme *tree, Lexeme *env)
    {
    eval((*tree).left, env);
    return eval((*tree).right, env);
    }

Lexeme *evalAnd(Lexeme *tree, Lexeme *env)
    {
    if ((*eval((*tree).left, env)).boolean == false)
        return newBooleanLexeme(false);
    else
        return newBooleanLexeme((*eval((*tree).right,env)).boolean);
    }

Lexeme *evalOr(Lexeme *tree, Lexeme *env)
    {
    if ((*eval((*tree).left, env)).boolean == true)
        return newBooleanLexeme(true);
    else
        return newBooleanLexeme((*eval((*tree).right,env)).boolean);
    }

Lexeme *evalLessThan(Lexeme *tree, Lexeme *env)
    {
    if ((*eval((*tree).left, env)).value < (*eval((*tree).right,env)).value)
        return newBooleanLexeme(true);
    else
        return newBooleanLexeme(false);
    }

Lexeme *evalGreaterThan(Lexeme *tree, Lexeme *env)
    {
    if ((*eval((*tree).left, env)).value > (*eval((*tree).right,env)).value)
        return newBooleanLexeme(true);
    else
        return newBooleanLexeme(false);
    }

Lexeme *evalIf(Lexeme *tree, Lexeme *env)
    {
    if ((*eval((*tree).left, env)).boolean == true)
        return eval((*(*tree).right).left, env);
    else
        return eval((*(*tree).right).right, env);
    }

Lexeme *evalWhile(Lexeme *tree, Lexeme *env)
    {
    while ((*eval((*tree).left, env)).boolean == true)
        eval((*tree).right, env);
    return NULL;
    }

void evalAssign(Lexeme *tree, Lexeme *env)
    {
	//error check
    if ((*(*tree).left).type != VARIABLE)
    	{
    	cerr << "Syntax error: Must assign with variable." << endl;
    	exit(-1);
    	}
    
    //eval the right hand side
    Lexeme *value = eval((*tree).right,env);
    updateEnv(env,(*tree).left,value);
    //return value;
    }

void printHelp(int i)
    {
    cout << i;
    }

void printHelp(string s)
    {
    if (s.compare("nLine") == 0)
        cout << endl;
    else
        cout << s;
    }

void printHelp(bool b)
    {
    if (b) cout << "true";
    else cout << "false";
    }

void evalPrint(Lexeme *eargs)
    {
    while (eargs != NULL)
        {
        Lexeme *check = car(eargs);
        if ((*check).type == INTEGER) printHelp((*check).value);
        else if ((*check).type == STRING) printHelp((*check).String);
        else if ((*check).type == BOOLEAN) printHelp((*check).boolean);
        else 
            {
            if (check != NULL)
                cout << printGRAM((*check).type);
            else 
                cerr << "Null lexeme.";
            }
        eargs = cdr(eargs);
        }
    }

Lexeme *isInt(Lexeme *eargs)
    {
    if ((*car(eargs)).type == INTEGER)
        return newBooleanLexeme(true);
    else 
        return newBooleanLexeme(false);
    }

Lexeme *evalMakeArray(Lexeme *eargs)
    {
    //error checking
    if (((*car(eargs)).type != INTEGER) || (cdr(eargs) != NULL))
    {
        cerr << "Invalid args in evalMakeArray." << endl;
        exit(-1);
    }

    int size = (*car(eargs)).value;
    if (size < 1)
        {
        cerr << "Size must be at least 1." << endl;
        exit(-1);
        }

    Lexeme *a = new Lexeme(ARRAY);
    (*a).array = vector<Lexeme*>(size, NULL);
    //a.aval = new Lexeme((*(*eargs).left).value);
    int i = 0;
    while (i < size)
        {
        (*a).array[i] = new Lexeme(UNDECLARED);
        i++; 
        }

    return a;
    }

Lexeme *evalGetArray(Lexeme *eargs)
    {
    Lexeme *a = car(eargs);
    Lexeme *place = car(cdr(eargs));
    Lexeme *emptyCheck = cdr(cdr(eargs));

    //error checking
    if (((*a).type != ARRAY) || ((*place).type != INTEGER) || (emptyCheck != NULL))
    {
        cerr << "Invalid args in evalGetArray." << endl;
        exit(1);
    }

    Lexeme *result = (*a).array[(*place).value];
    return result;
    }

void evalSetArray(Lexeme *eargs)
    {
    Lexeme *a = car(eargs);
    Lexeme *place = car(cdr(eargs));
    Lexeme *setValue = car(cdr(cdr(eargs)));

    //error checking
    if (((*a).type != ARRAY) || ((*place).type != INTEGER) || (cdr(cdr(cdr(eargs))) != NULL))
        {
        cerr << "Invalid args in evalSetArray." << endl;
        exit(1);
        }

    (*a).array[(*place).value] = setValue;
    }

//to test array elements
void viewArray(Lexeme *a)
    {
    if ((*a).array[0] == NULL)
        return;
    else
        cout << printGRAM((*(*a).array[0]).type) << endl;
    }

//to test array elements
void showArray(Lexeme *eargs)
    {
    Lexeme *a = car(eargs);
    Lexeme *size = car(cdr(eargs));
    int s = (*size).value;
    cout << printGRAM((*a).type) << endl;

    int i = 0;
    while (i < s)
        {
        (*(*a).array[i]).display();
        i++;    
        }
    }













