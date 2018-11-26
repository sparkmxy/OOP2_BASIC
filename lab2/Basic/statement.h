/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "../StanfordCPPLib/tokenscanner.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;
/*
 * Method: parseSta
 * Usage: stmt->parseSta(scanner);
 * ----------------------------
 * This method parses a BASIC statement from the scanner.The scanner should be set to ignore
 * whitespace and to scan numbers.
 */
   virtual void parseSta(TokenScanner &scanner) = 0;
};

/*
 * Class: LET_Sta
 * ------------------
 * This subclass represents a statement used for LET assignment
 */
class LET_Sta : public Statement {
public:
	/*
	 * Constructor: REM_Sta
	 * Usage: Statement *statement = new LET_Sta(varName,value);
	 * ------------------------------------------------
	 * The constructor initializes a Let statement with a varName and the
	 * value needs to be assign to it
	 */
	LET_Sta() = default;
	LET_Sta(string varName,Expression *exp);

	/*
	 * Prototypes for the virtual methods
	 * ----------------------------------
	 * These methods have the same prototypes as those in the Expression
	 * base class and don't require additional documentation.
	 */
	virtual void execute(EvalState & state);
	virtual void parseSta(TokenScanner &scanner);
private:
	string varName;
	Expression *exp;
};

/*
 * Class: PRINT_Sta
 * ------------------
 * This subclass represents a statement used for PRINT Statement
 */
class PRINT_Sta : public Statement{
public:
	/*
	 * Constructor: PRINT_Sta
	 * Usage: Statement *statement = new PRINT_Sta(*exp);
	 * ------------------------------------------------
	 * The constructor initializes a PRINT statement with a pointer to an Expression
	 */
	PRINT_Sta() = default;
	PRINT_Sta(Expression *exp);
	/*
	 * Prototypes for the virtual methods
	 * ----------------------------------
	 * These methods have the same prototypes as those in the Expression
	 * base class and don't require additional documentation.
	 */
	virtual ~PRINT_Sta();
	virtual void execute(EvalState & state);
	virtual void parseSta(TokenScanner &scanner);
private:
	Expression *exp;
};

/*
 * Class: INPUT_Sta
 * ------------------
 * This subclass represents a statement used for INPUT statement
 */
class INPUT_Sta: public Statement{
public:
	/*
	 * Constructor: INPUT_Sta
	 * Usage: Statement *statement = new INPUT_Sta(varName);
	 * ------------------------------------------------
	 * The constructor initializes a INPUT statement with a varName 
	 */
	INPUT_Sta() = default;
	INPUT_Sta(string varName);
	/*
	 * Prototypes for the virtual methods
	 * ----------------------------------
	 * These methods have the same prototypes as those in the Expression
	 * base class and don't require additional documentation.
	 */
	virtual void execute(EvalState & state);
	virtual void parseSta(TokenScanner &scanner);
private:
	string varName;
};

/*
 * Class: End_Statement
 * ------------------
 * This subclass represents a statement used for the End comment
 */
class END_Sta :public Statement {
public:
	/*
	 * Constructor: INPUT_Sta
	 * Usage: Statement *statement = new END_Sta();
	 * ------------------------------------------------
	 * The constructor initializes a END statemenT
	 */
	END_Sta() = default;
	/*
	 * Prototypes for the virtual methods
	 * ----------------------------------
	 * These methods have the same prototypes as those in the Expression
	 * base class and don't require additional documentation.
	 */
	virtual void execute(EvalState & state);
	virtual void parseSta(TokenScanner &scanner);
};

/*
 * Class: GOTO_Sta
 * ------------------
 * This subclass represents a statement used for GOTO statement
 */
class GOTO_Sta :public Statement {
public:
	/*
	 * Constructor: GOTO_Sta
	 * Usage: Statement *statement = new GOTO_Sta(lineNumber);
	 * ------------------------------------------------
	 * The constructor initializes a GOTO statement with a lineNumer
	 */
	GOTO_Sta() = default;
	GOTO_Sta(int lineNumber);
	/*
	 * Prototypes for the virtual methods
	 * ----------------------------------
	 * These methods have the same prototypes as those in the Expression
	 * base class and don't require additional documentation.
	 */
	virtual void execute(EvalState & state);
	virtual void parseSta(TokenScanner &scanner);
private:
	int lineNumber;
};

/*
 * Class: IF_Sta
 * ------------------
 * This subclass represents a statement used for IF statement
 */
class IF_Sta :public Statement {
public:
	/*
	 * Constructor: INPUT_Sta
	 * Usage: Statement *statement = new INPUT_Sta(varName);
	 * ------------------------------------------------
	 * The constructor initializes a INPUT statement with a tow Expressions
	 * ,an operator for comparison and a lineNumber
	 */
	IF_Sta() = default;
	IF_Sta(char op,Expression *lhs,Expression *rhs,int lineNumber);
	~IF_Sta();
	/*
	 * Prototypes for the virtual methods
	 * ----------------------------------
	 * These methods have the same prototypes as those in the Expression
	 * base class and don't require additional documentation.
	 */
	virtual void execute(EvalState & state);
	virtual void parseSta(TokenScanner &scanner);
private:
	Expression *lhs, *rhs;
	char op;
	int lineNumber;
};

 /*
  * Function: getStatement
  * Usage: Expression *stmt = getStatement(scanner);
  * -------------------------------------------
  * Parses an Statement by reading tokens from the scanner, which must
  * be provided by the client.  The scanner should be set to ignore
  * whitespace and to scan numbers.
  */
Statement *getStatement(TokenScanner &scanner);

#endif
