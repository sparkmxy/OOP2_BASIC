/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "parser.h"

using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
} 

Statement::~Statement() {
   /* Empty */
}

/*
 * Implementation notes: the Let_Sta subclass
 * ----------------------------------------------
 * The LET_Sta subclass declares Statement for assigning a variable.
 * Note that some names can not be used
 */
static const int illegalCnt = 13;
static const string illegalNames[illegalCnt] = {
	"LET","REM","GOTO","IF","THEN","INPUT","PRINT","END","LIST","RUN","QUIT","HELP","CLEAR"
}; 
	
LET_Sta::LET_Sta(string varName,Expression *exp) : varName(varName), exp(exp) {}

void LET_Sta::execute(EvalState & state) {
	state.setValue(varName, exp->eval(state));
}

void LET_Sta::parseSta(TokenScanner &scanner) {
	varName = scanner.nextToken();
	TokenType type = scanner.getTokenType(varName);
	int id = 0;
	while(id < illegalCnt && illegalNames[id] != varName) id++;
	if (type != WORD || scanner.nextToken() != "=" || id != illegalCnt) 
		error("SYNTAX ERROR");
	exp = parseExp(scanner);
}

/*
 * Implementation notes: the PRINT_Sta subclass
 * ----------------------------------------------
 * The PRINT_Sta subclass declares Statement for printing the value of and expression.
 */

PRINT_Sta::PRINT_Sta(Expression *exp) : exp(exp) {}

PRINT_Sta::~PRINT_Sta() {
	delete exp;
}

void PRINT_Sta::execute(EvalState &state) {
	cout << exp->eval(state) << endl;
}

void PRINT_Sta::parseSta(TokenScanner &scanner) {
	try {
		exp = parseExp(scanner);
	}
	catch (...) {
		error("SYNTAX ERROR");
	}
}
/*
 * Implementation notes: the INPUT_Sta subclass
 * ----------------------------------------------
 * The INPUT_Sta subclass declares Statement for requiring input of a variable.
 */

INPUT_Sta::INPUT_Sta(string varName) :varName(varName) {}

void INPUT_Sta::execute(EvalState &state) {
	string str;
	bool ok = false;
	int value;
	while (1) {
		cout << " ? ";
		cin >> str;
		ok = true;
		try {
			value = stringToInteger(str);
		}
		catch (...) {
			ok = false;
		}
		if (ok) break;
		cout << "INVALID NUMBER\n";
	}
	state.setValue(varName, value);
}

void INPUT_Sta::parseSta(TokenScanner &scanner) {
	varName = scanner.nextToken();
	//cout << "input varName: " << varName << endl;
	if(scanner.getTokenType(varName)!=WORD)
		error("SYNTAX ERROR");
}
/*
 * Implementation notes: the END_Sta subclass
 * ----------------------------------------------
 * The END_Sta subclass declares Statement for creating a tag stating the program 
 * has ended
 */

void END_Sta::execute(EvalState &state) {
	state.setValue("PROGRAM_END_TAG", 1);
}
void END_Sta::parseSta(TokenScanner &scanner) {}

/*
 * Implementation notes: the GOTO_Sta subclass
 * ----------------------------------------------
 * The GOTO_Sta subclass declares Statement for excuting the program from a given
 * line,by creating a GOTO_TAG
 */
GOTO_Sta::GOTO_Sta(int lineNumber) :lineNumber(lineNumber) {}

void GOTO_Sta::execute(EvalState &state) {
	state.setValue("GOTO_TAG", lineNumber);
	state.setValue("GOTO_DONE", 0);
}

void GOTO_Sta::parseSta(TokenScanner &scanner) {
	string num = scanner.nextToken();
	if (scanner.getTokenType(num) != NUMBER) error("SYNTAX ERROR");
	lineNumber = stringToInteger(num);
}
/*
 * Implementation notes: the IF_Sta subclass
 * ----------------------------------------------
 * The IF_Sta subclass declares Statement for IF-THEN clause
 */
IF_Sta::IF_Sta(char op, Expression *lhs, Expression *rhs, int lineNumber) :
	op(op), lhs(lhs), rhs(rhs), lineNumber(lineNumber) {}

IF_Sta::~IF_Sta() {
	delete lhs;
	delete rhs;
}

void IF_Sta::execute(EvalState &state) {
	int l = lhs->eval(state), r = rhs->eval(state);
	if ((op == '=' && l == r) || (op == '<' && l < r) || (op == '>' && l > r)) {
		state.setValue("GOTO_TAG", lineNumber);
		state.setValue("GOTO_DONE", 0);
	}
}

void IF_Sta::parseSta(TokenScanner &scanner) {
	string left = "";
	do {
		string nxt = scanner.nextToken();
		if (nxt != "=" && nxt != "<" && nxt != ">") left += nxt;
		else {
			scanner.saveToken(nxt);
			break;
		}
		if (scanner.getTokenType(nxt) == EOF) error("SYNTAX ERROR");
	} while (1);
	//cout << "if left: " << left << endl;
	try {
		TokenScanner temp;
		temp.ignoreWhitespace();
		temp.scanNumbers();
		temp.setInput(left);
		lhs = parseExp(temp);
	}
	catch (...) {
		error("SYNTAX ERROR");
	}
	op = scanner.nextToken()[0];
	string right = "";
	do {
		string nxt = scanner.nextToken();
		if (nxt != "THEN") right += nxt;
		else
			break;
		if (scanner.getTokenType(nxt) == EOF) error("SYNTAX ERROR");
	} while (1);
	//cout << "if right: " << right << endl;
	try {
		TokenScanner temp;
		temp.ignoreWhitespace();
		temp.scanNumbers();
		temp.setInput(right);
		rhs = parseExp(temp);
	}
	catch (...) {
		error("SYNTAX ERROR");
	}
	string num = scanner.nextToken();
	if (scanner.getTokenType(num) == NUMBER)
		lineNumber = stringToInteger(num);
	else 
		error("SYNTAX ERROR");
}
/*
 * Implementation notes: getStatement
 * ------------------------------
 * This code just reads a statement.
 */

Statement *getStatement(TokenScanner &scanner) {
	static const int SIZE = 7;
	static string Types[SIZE] = { "REM","LET","PRINT","INPUT","GOTO","IF","END" };
	string type = scanner.nextToken();
	int id = 0;
	while (Types[id] != type && id < SIZE) id++;
	Statement *stmt = nullptr;
	switch (id){
		case 0: break;
		case 1: stmt = new LET_Sta; break;
		case 2: stmt = new PRINT_Sta; break;
		case 3: stmt = new INPUT_Sta; break;
		case 4: stmt = new GOTO_Sta; break;
		case 5: stmt = new IF_Sta; break;
		case 6: stmt = new END_Sta; break;
		default:
			error("SYNTAX ERROR");
	}
	if (stmt != nullptr) stmt->parseSta(scanner);
	//stmt->execute(EvalState());
	if (id!=0 && scanner.hasMoreTokens()) error("SYNTAX ERROR");
	return stmt;
}



