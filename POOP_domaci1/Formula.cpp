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
	try {
		string dereferenced = dereferenceCells();
		string postfix = Formula::infixToPostfix(dereferenced);
		double result = Formula::calculatePostfix(postfix);
		return result;
	}
	catch (DivisionByZero& err) {
		invalidExpression = true;
		return 0;
	}
	catch (FormulaParameterNotNumber& err) {
		invalidExpression = true;
		return 0;
	}
	catch (ExpressionNotValid& err) {
		invalidExpression = true;
		return 0;
	}
	catch (CellNotExists& err) {
		invalidExpression = true;
		return 0;
	}
	catch (RecursiveReference& err) {
		invalidExpression = true;
		return 0;
	}
}

string Formula::getFormulaValueAsString() {
	stringstream stream;
	stream << calculateFormula();
	if (invalidExpression) return "\033[1;31mERROR\033[0m";
	return stream.str();
}


string Formula::dereferenceCells() const {
	stringstream infix;
	stringstream cellName;
	for (char c : this->expression) {
		if (c == '=') continue;
		if (isalpha(c)) {
			cellName << c;
		}
		else if (isdigit(c) && cellName.str() != "") {
			cellName << c;
		}
		else {
			if (cellName.str() != "") {
				Cell* cell = table->getCell(cellName.str());
				if (cell && cell->getInputValue() == expression) throw RecursiveReference();
				if (cell == nullptr && table->columnFormats[toupper(cellName.str()[0]) - 65] != 'N') throw FormulaParameterNotNumber();
				if (cell && cell->getFormat() != 'N') throw FormulaParameterNotNumber();
				cellName.str("");
				infix << (cell ? ((NumberCell*)cell)->getNumberValue() : 0) << c;
			}
			else {
				infix << c;
			}
		}
	}
	if (cellName.str() != "") {
		Cell* cell = table->getCell(cellName.str());
		if (cell && cell->getInputValue() == expression) throw RecursiveReference();
		if (cell == nullptr && table->columnFormats[toupper(cellName.str()[0]) - 65] != 'N') throw FormulaParameterNotNumber();
		if (cell && cell->getFormat() != 'N') throw FormulaParameterNotNumber();
		cellName.str("");
		infix << (cell ? ((NumberCell*)cell)->getNumberValue() : 0);
	}
	return infix.str();
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
