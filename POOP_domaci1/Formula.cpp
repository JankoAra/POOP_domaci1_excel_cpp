#include "Formula.h"
#include <regex>
#include <sstream>
#include <vector>
#include "Errors.h"
#include <stack>
#include <map>
#include <set>
#include <cctype>
#include "NumberCell.h"

Formula::Formula(string exp, Table* t) {
	table = t;
	regex blanks("\\s+");
	expression = regex_replace(exp, blanks, "");
}

double Formula::calculateFormula() {
	string dereferenced = dereferenceCells();
	string postfix = Formula::infixToPostfix(dereferenced);
	double result = Formula::calculatePostfix(postfix);
	return result;
}

string Formula::getFormulaValueAsString() {
	stringstream stream;
	stream << calculateFormula();
	if (invalidExpression) return "\033[1;31mERROR\033[0m";
	return stream.str();
}

string Formula::dereferenceCells() const{
	//podela izraza na tokene
	set<char> ops = { '+', '-', '*', '/', '(', ')', '=' };
	vector<string> tokens;
	stringstream stream("");
	for (char c : this->expression) {
		if (ops.count(c)) {
			if (!stream.str().empty()) {
				tokens.push_back(stream.str());
				stream.str("");
			}
			tokens.push_back(string(1, c));
		}
		else {
			stream << c;
		}
	}
	if (!stream.str().empty()) {
		tokens.push_back(stream.str());
		stream.str("");
	}

	//provera da li su tokeni ispravni
	set<string> opsString = { "+", "-", "*", "/", "(", ")", "=" };
	auto isNumber = [](string s) {
		regex pattern("(^\\d*\\.?\\d+$|^\\d+\\.?\\d*$)");
		return regex_match(s, pattern);
	};
	auto isCellName = [](string s) {
		regex pattern("^[A-Za-z]\\d+$");
		return regex_match(s, pattern);
	};
	for (int i = 0; i < tokens.size(); i++) {
		string& tok = tokens[i];
		if (tok == "=" && i != 0) throw ExpressionNotValid();
		if (opsString.count(tok)) continue;
		if (isNumber(tok)) continue;
		if (isCellName(tok)) {
			// referencirana celija je sigurno broj
			NumberCell* cell = (NumberCell*)table->getCell(tok);
			double val = cell ? cell->getNumberValue() : 0;
			stream << val;
			tok = stream.str();
			stream.str("");
			continue;
		}
		//neka greska
		throw CellNotExists();
	}
	for (string& tok : tokens) {
		if (tok != "=") stream << tok;
	}
	return stream.str();
}

string Formula::infixToPostfix(string expression) {
	//definisanje prioriteta operanada
	// @ je unarni -
	// # je unarni +

	map<string, int> inputPriority = { {"+",2},{"-",2},{"*",3},{"/",3},{"@",5},{"#",5}, {"(",10},{")",1} };
	map<string, int> stackPriority = { {"+",2},{"-",2},{"*",3},{"/",3},{"@",4},{"#",4}, {"(",0} };
	set<string> ops = { "+", "-", "*", "/", "(", ")", "@", "#" };
	int nextToRead = 0;
	stack<string> stack;
	int rank = 0;	//kontrola ispravnosti izraza
	string postfix;
	string next = getNextToken(expression, nextToRead);
	while (next != "") {
		if (ops.find(next) == ops.end()) {
			//token je operand, ide direktno u postfix izraz
			postfix += next + " ";
			rank++;
		}
		else {
			//token je operator
			while (!stack.empty() && (inputPriority[next] <= stackPriority[stack.top()])) {
				//vadimo operatore veceg prioriteta sa steka
				string op = stack.top();
				stack.pop();
				postfix += op + " ";
				rank = rank + ((op == "@" || op == "#") ? 0 : -1);
				if (rank < 1) throw ExpressionNotValid();
			}
			if (next != ")") {
				stack.push(next);
			}
			else {
				stack.pop();
			}
		}
		next = getNextToken(expression, nextToRead);
	}
	//procitan ceo infiks, popujemo ostatak sa steka
	while (!stack.empty()) {
		string op = stack.top();
		stack.pop();
		postfix += op + " ";
		rank = rank + ((op == "@" || op == "#") ? 0 : -1);
	}
	if (rank != 1) throw ExpressionNotValid();
	return postfix;
}

double Formula::calculatePostfix(string postfix) {
	stack<string> stack;
	int index = 0;
	string next = getNextTokenPostfix(postfix, index);
	set<string> ops = { "+", "-", "*", "/", "(", ")", "@", "#" };
	while (next != "") {
		if (ops.find(next) == ops.end()) {
			//operandi idu na stek
			stack.push(next);
		}
		else if (next == "@" || next == "#") {
			//unarni operatori skinu jedan operand sa steka i vrate novu vrednost
			double op = stod(stack.top());
			if (next == "@") op = -op;
			stringstream stream;
			stream << op;
			stack.pop();
			stack.push(stream.str());
		}
		else {
			//binarni operandi uzmu dve vrednosti sa steka i vrate jednu
			double op2 = stod(stack.top());
			stack.pop();
			double op1 = stod(stack.top());
			stack.pop();
			stringstream stream;
			double res = 0;
			if (next == "+") res = op1 + op2;
			else if (next == "-") res = op1 - op2;
			else if (next == "*") res = op1 * op2;
			else if (next == "/") {
				if (op2 == 0) {
					throw DivisionByZero();
				}
				res = op1 / op2;
			}
			stream << res;
			stack.push(stream.str());
		}
		next = getNextTokenPostfix(postfix, index);
	}
	double result = stod(stack.top());
	stack.pop();
	if (stack.empty()) return result;
	else throw ExpressionNotValid();
}

string Formula::getNextToken(string& input, int& index) {
	stringstream stream;
	set<char> ops = { '+','-','*','/','(',')' };
	while (index < input.length()) {
		char currentChar = input[index];
		if (ops.find(currentChar) != ops.end()) {
			//naisli smo na operator
			if (stream.str() != "") {
				//ako smo pre toga citali operand, prvo vracamo operand
				return stream.str();
			}
			//da li je operator unarni?
			if ((index > 0 && (currentChar == '+' || currentChar == '-') && \
				ops.find(input[index - 1]) != ops.end() && input[index - 1] != ')')\
				|| (index == 0 && (currentChar == '+' || currentChar == '-'))) {
				//jeste unarni
				stream << (currentChar == '+' ? "#" : "@");
				index++;
				return stream.str();
			}
			//nije unarni operator
			stream << currentChar;
			index++;
			return stream.str();
		}
		//nailazimo na operand
		stream << currentChar;
		index++;
	}
	return stream.str();
}

string Formula::getNextTokenPostfix(string& postfix, int& index) {
	stringstream stream;
	while (index < postfix.length()) {
		char currentChar = postfix[index++];
		if (currentChar == ' ') break;
		stream << currentChar;
	}
	return stream.str();
}
