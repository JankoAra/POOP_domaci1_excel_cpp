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
	//moguce samo ispravne formule
	Formula f(inputValue, myTable);
	return f.calculateFormula();
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
		bool circularReferenceExists = false;
		bool unallowedReferenceExists = false;	//flag da li se referencira celija koja nije formatirana kao broj
		try {
			circularReferenceExists = NumberCell::hasCircularRefrence((NumberCell*)this, visited);
		}
		catch (FormulaParameterNotNumber& err) {
			unallowedReferenceExists = true;
		}
		if (circularReferenceExists || unallowedReferenceExists) return "ERROR";

		// i dalje u izrazu moze biti nepostojecih celija ili gresaka u izrazu(deljenje nulom, neuparene zagrade...)
		try {
			formulaValue = f.calculateFormula();
		}
		catch (FormulaCalculationError& err) {
			return "ERROR";
		}
		catch (CellNotExists& err) {
			return "ERROR";
		}
		stream << fixed << setprecision(this->decimalSpaces) << formulaValue;
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

vector<NumberCell*> NumberCell::getReferencedCells() const {
	set<string> refCellNames;
	vector<NumberCell*> refCells;
	if (inputValue[0] != '=') return refCells;
	regex cellNamePattern("(^|[^A-Za-z0-9])([A-Za-z]\\d+)($|[^A-Za-z0-9])");
	sregex_iterator iter(inputValue.begin(), inputValue.end(), cellNamePattern);
	sregex_iterator end;

	while (iter != end) {
		smatch match = *iter;
		string cellName = match[2].str();
		cellName[0] = toupper(cellName[0]);
		refCellNames.insert(cellName);
		iter++;
	}
	for (auto& cellName : refCellNames) {
		Cell* cell = myTable->getCell(cellName);
		char format = cell == nullptr ? myTable->getColumnFormats()[cellName[0] - 65] : cell->getFormat();
		if (format != 'N') throw FormulaParameterNotNumber();
		if (cell == nullptr) continue;
		refCells.push_back((NumberCell*)cell);
	}
	return refCells;
}

bool NumberCell::hasCircularRefrence(NumberCell* cell, vector<NumberCell*>& visited) {
	visited.push_back(cell);
	for (NumberCell* c : cell->getReferencedCells()) {
		if (find(visited.begin(), visited.end(), c) != visited.end()) return true;

		if (hasCircularRefrence(c, visited)) return true;
	}
	visited.pop_back();
	return false;
}