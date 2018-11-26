/*
 * File: Basic.cpp
 * ---------------
 * Name: [Mao Xinyu]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   cout << "Stub implementation of BASIC" << endl;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user. In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
	if (line == "") return;
	static const int SIZE = 5;
	static string cmd[SIZE] = { "RUN","LIST","HELP","QUIT","CLEAR"};
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	string fst = scanner.nextToken();
	int c = 0;
	while (c < SIZE && cmd[c] != fst) c++;
	if (c < SIZE) {
		if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
		switch (c) {
			case 0: program.run(state); break;
			case 1: program.display(); break;
			case 2: cout << "Nobody can help you!" << endl; break;
			case 3: exit(0);
			case 4: program.clear(); state.clear();
		}
	}
	else if (scanner.getTokenType(fst) == NUMBER) {
		if (scanner.hasMoreTokens())
			program.addSourceLine(stringToInteger(fst), line);
		else
			program.removeSourceLine(stringToInteger(fst));
	}
	else {
		if(fst!="PRINT" && fst!="LET" && fst!="INPUT")
			error("SYNTAX ERROR");
		Statement *stmt = nullptr;
		if (fst == "PRINT") stmt = new PRINT_Sta;
		else if (fst == "LET") stmt = new LET_Sta;
		else stmt = new INPUT_Sta;
		stmt->parseSta(scanner);
		stmt->execute(state);
		delete stmt;
	}
}
