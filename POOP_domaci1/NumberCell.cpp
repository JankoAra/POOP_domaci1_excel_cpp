#include "NumberCell.h"
#include <sstream>
#include <regex>
#include <iomanip>
#include "Errors.h"
#include "Formula.h"
#include <set>

double NumberCell::getNumberValue() const {
	if (inputValue == "") return 0;
	if (inputValue[0] != '=') {
		//nije formula u pitanju
		return stod(inputValue);
	}
	Formula f(inputValue, myTable);
	vector<NumberCell*> visited;
	double formulaValue = 0;
	if (f.hasCircularRefrence((NumberCell*)this, visited)) f.setInvalid(true);
	else formulaValue = f.calculateFormula();
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
		vector<NumberCell*> visited;
		double formulaValue = 0;
		if (f.hasCircularRefrence((NumberCell*)this, visited)) f.setInvalid(true);
		else formulaValue = f.calculateFormula();
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

vector<NumberCell*> NumberCell::getReferencedCells() const{
	set<string> refCellNames;
	vector<NumberCell*> refCells;
	if (inputValue[0] != '=') return refCells;
	regex cellNamePattern("(#|[^A-Za-z0-9])([A-Za-z]\\d+)($|[^A-Za-z0-9])");
	sregex_iterator iter(inputValue.begin(), inputValue.end(), cellNamePattern);
	sregex_iterator end;

	while (iter != end) {
		smatch match = *iter;
		string cellName = match[2].str();
		cellName[0] = toupper(cellName[0]);
		refCellNames.insert(cellName);
		iter++;
	}
	for (auto& a : refCellNames) {
		Cell* cell = myTable->getCell(a);
		char format = cell==nullptr ? myTable->getColumnFormats()[a[0]-65] : cell->getFormat();
		if (format != 'N') throw ExpressionNotValid();
		if (cell == nullptr) continue;
		refCells.push_back((NumberCell*)cell);
	}
	return refCells;
}
