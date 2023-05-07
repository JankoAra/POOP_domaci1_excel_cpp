#include "DateCell.h"
#include <set>
#include <sstream>
#include <regex>
#include "Errors.h"
#include <iomanip>
void DateCell::setInputValue(string val) {
	regex blanks("\\s+");
	if (regex_match(val, blanks) || val == "") {
		inputValue = "";
		return;
	}
	regex datePattern("^\\s*(\\d{1,2})\\.(\\d{1,2})\\.(\\d{4})\\.\\s*$");
	if (!regex_match(val, datePattern)) throw CellInputError();
	string removedBlanks = regex_replace(val, blanks, "");
	smatch match;
	regex_search(removedBlanks, match, datePattern);
	int day = stoi(match[1].str());
	int month = stoi(match[2].str());
	int year = stoi(match[3].str());
	if (!checkValidDate(day, month, year)) throw InvalidDate();
	ostringstream stream;
	stream << setfill('0') << setw(2) << day << "."\
		<< setfill('0') << setw(2) << month << "."\
		<< setfill('0') << setw(4) << year << ".";
	inputValue = stream.str();
}

bool DateCell::validInputForFormat(string input){
	return staticValidInput(input);
}

bool DateCell::checkValidDate(int d, int m, int y) {
	if (d <= 0 || m <= 0 || y <= 0 || m > 12 || d > 31) return false;
	//set<int> thirty = { 4,6,9,11 };
	set<int> thirtyOne = { 1,3,5,7,8,10,12 };
	if (d == 31 && thirtyOne.count(m) == 0) return false;
	if (m == 2 && d <= 28) return true;
	if (m == 2 && d == 29) {
		if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) return true;
		return false;
	}
	if (m == 2 && d > 29) return false;
	return true;
}

bool DateCell::staticValidInput(string input){
	regex blanks("\\s+");
	if (regex_match(input, blanks) || input == "") return true;
	regex datePattern("^\\s*(\\d{1,2})\\.(\\d{1,2})\\.(\\d{4})\\.\\s*$");
	if (!regex_match(input, datePattern)) return false;
	string removedBlanks = regex_replace(input, blanks, "");
	smatch match;
	regex_search(removedBlanks, match, datePattern);
	int day = stoi(match[1].str());
	int month = stoi(match[2].str());
	int year = stoi(match[3].str());
	if (!checkValidDate(day, month, year)) throw InvalidDate();
	return true;
}
