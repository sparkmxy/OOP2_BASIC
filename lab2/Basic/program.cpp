/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() {
}

Program::~Program() {
}

void Program::clear() {
	S.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	scanner.nextToken();
	clause now = clause(lineNumber, line, getStatement(scanner));
	auto it = S.find(now);
	if (it == S.end()) S.insert(now);
	else {
		S.erase(it);
		S.insert(now);
	}
}

void Program::removeSourceLine(int lineNumber) {
	auto it = S.find(clause(lineNumber));
	if (it != S.end()) S.erase(it);
}

string Program::getSourceLine(int lineNumber) {
	return S.find(clause(lineNumber))->line;
}

int Program::getFirstLineNumber() {
	return S.begin()->lineNumber;
}

int Program::getNextLineNumber(int lineNumber) {
	auto it = S.find(clause(lineNumber));
	it++;
	return it->lineNumber; 
}

void Program::display() {
	for (auto it = S.begin(); it != S.end(); it++)
		cout << it->line << endl;
}

void Program::run(EvalState &state) {
	state.setValue("GOTO_DONE", 1);
	state.setValue("PROGRAM_END_TAG",0);
	for (auto it = S.begin(); it != S.end() ||  state.getValue("GOTO_DONE") == 0 ;) {
	//	cout << "Run Clause:" << it->lineNumber << endl;
		if (state.getValue("PROGRAM_END_TAG") == 1) return;
		if (state.getValue("GOTO_DONE") == 0) {
			state.setValue("GOTO_DONE", 1);
			int val = state.getValue("GOTO_TAG");
			auto itt = S.find(clause(val));
			if (itt == S.end())
				error("LINE NUMBER ERROR");
			else it = itt;
		}
		else {
			if (it->stmt != nullptr)it->stmt->execute(state);
			it++;
		}
	}
}
