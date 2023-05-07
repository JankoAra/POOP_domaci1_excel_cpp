#include "NumberCell.h"
#include <sstream>
#include <regex>
#include <iomanip>
#include "Errors.h"
#include "Formula.h"

double NumberCell::getNumberValue() const {
	if (inputValue == "") return 0;
	if (inputValue[0] != '=') {
		//nije formula u pitanju
		return stod(inputValue);
	}
	Formula f(inputValue, myTable);
	double formulaValue = f.calculateFormula();
	if (f.isInvalid()) {
		throw ExpressionNotValid();
	}
	else {
		return formulaValue;
	}
}

string NumberCell::getFormattedValue() const {
	if (inputValue == "") return inputValue;
	ostringstream stream;
	if (inputValue[0] != '=') {
		//nije formula u pitanju
		double cellValueDouble = stod(inputValue);
		stream << fixed << setprecision(this->decimalSpaces) << cellValueDouble;
	}
	else {
		Formula f(inputValue, myTable);
		double formulaValue = f.calculateFormula();
		if (f.isInvalid()) {
			stream << "ERROR";
		}
		else {
			stream << fixed << setprecision(this->decimalSpaces) << formulaValue;
		}
	}
	return stream.str();
}

void NumberCell::setInputValue(string val) {
	regex numberPattern("^\\s*\\d*\\.?\\d*\\s*$");
	regex expressionPattern("^\\s*=");
	if (!regex_match(val, numberPattern) && !regex_search(val, expressionPattern)) throw CellInputError();
	regex blank("\\s+");
	string removedBlanks = regex_replace(val, blank, "");
	inputValue = removedBlanks;
}

bool NumberCell::staticValidInput(string input) {
	regex numberPattern("^\\s*\\d*\\.?\\d*\\s*$");
	regex expressionPattern("^\\s*=");
	if (!regex_match(input, numberPattern) && !regex_search(input, expressionPattern)) return false;
	return true;
}
