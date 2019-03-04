#include "env.h"
using namespace std;

Lexeme *cons(GRAMTYPE type, Lexeme *carValue, Lexeme *cdrValue) 
	{ 
	Lexeme *result = new Lexeme(type);
	(*result).carValue = carValue;
	(*result).cdrValue = cdrValue;
	return result;
	}

Lexeme *car(Lexeme *cell) { return (*cell).carValue; }
Lexeme *cdr(Lexeme *cell) { return (*cell).cdrValue; }
void setCar(Lexeme *cell, Lexeme *value) { (*cell).carValue = value; }
void setCdr(Lexeme *cell, Lexeme *value) { (*cell).cdrValue = value; }

bool sameVariable(Lexeme *x, Lexeme *y)
    {
    if ((*x).String.compare((*y).String) == 0) return true;
    else 
        return false;
    }

Lexeme *create(void)
    {
    return extendEnv(NULL, NULL, NULL);
    }

Lexeme *makeTable(Lexeme *variables, Lexeme *values)
    {
    return cons(TABLE,variables,values);
    }

Lexeme *extendEnv(Lexeme *env, Lexeme *variables, Lexeme *values)
    {
    return cons(ENV,makeTable(variables,values),env);
    }

Lexeme *insertEnv(Lexeme *variable, Lexeme *value, Lexeme *env)
    {
    Lexeme *table = car(env);
    setCar(table,cons(GLUE,variable,car(table)));
    setCdr(table,cons(GLUE,value,cdr(table)));
    return table;
    }

Lexeme *lookupEnv(Lexeme *variable, Lexeme *env)
    {
    while (env != NULL)
        {
        Lexeme *table = car(env);
        Lexeme *vars = car(table);
        Lexeme *vals = cdr(table);
        while (vars != NULL)
            {
            if (sameVariable(variable,car(vars)))
                {
                return car(vals);
                }
            vars = cdr(vars);
            vals = cdr(vals);
            }
        env = cdr(env);
        }

    //cout << "variable " << variable << " is undefined" << endl;
    cout << "variable " << (*variable).String << " is undefined" << endl;

    return NULL;
    }

void updateEnv(Lexeme *env, Lexeme *variable, Lexeme *value)
	{
	while (env != NULL)
        {
        Lexeme *table = car(env);
        Lexeme *vars = car(table);
        Lexeme *vals = cdr(table);
        while (vars != NULL)
            {
            if (sameVariable(variable,car(vars)))
                {
                setCar(vals, value);
                return;
                }
            vars = cdr(vars);
            vals = cdr(vals);
            }
        env = cdr(env);
        }

    //cout << "variable " << variable << " is undefined" << endl;
   	cout << "variable " << (*variable).String << " is undefined" << endl;
	}

//for testing
void displayEnv(Lexeme *env)
    {
    cout << "Displaying Environment:" << endl;
    while (env != NULL) 
        {
        cout << "----------------" << endl;
        Lexeme *table = car(env);
        Lexeme *vars = car(table);
        Lexeme *values = cdr(table);

        while (vars != NULL)
            {
            cout << "var: " << (*car(values)).String;
            if ((*car(values)).type == INTEGER) cout << (*car(values)).value << endl;
            else if ((*car(values)).type == STRING) cout << (*car(values)).String << endl;
            else cout << printGRAM((*car(values)).type) << endl;

            vars = cdr(vars);
            values = cdr(values);
            }
        cout << "----------------" << endl;
        env = cdr(env);
        }
    //finished displaying environment
    }

