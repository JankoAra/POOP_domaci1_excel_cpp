#ifndef _ACTION_RECORD_STRUCT
#define _ACTION_RECORD_STRUCT

#include <vector>
#include <map>
#include <string>
#include "Cell.h"

using namespace std;

struct ActionRecord {
	enum ActionType { CHANGE_CELL, FORMAT_ROW, FORMAT_COLUMN } actionType;

	int row = 0;
	int column = 0;
	string oldCellValue;
	string newCellValue;
	char oldCellFormat = 0;
	char newCellFormat = 0;
	map<int, char> oldColumnFormats;
	vector<int> oldColumnDecimals;
	vector<int> newColumnDecimals;
	char oldGlobalColumnFormat = 0;
	char newGlobalColumnFormat = 0;
	vector<char> oldRowFormats;
	vector<char> newRowFormats;
	int oldDecimalsToSet = 0;
	int newDecimalsToSet = 0;

	//default konstruktor
	ActionRecord() {}
	//konstruktor za change_cell
	ActionRecord(ActionType type, int r, int c, string oldV, string newV, char oldForm, char newForm) {
		actionType = type;
		row = r;
		column = c;
		oldCellValue = oldV;
		newCellValue = newV;
		oldCellFormat = oldForm;
		newCellFormat = newForm;
	}
	ActionRecord(ActionType type, int r, int c, string oldV, string newV, char oldForm, char newForm, int oldDec, int newDec) {
		actionType = type;
		row = r;
		column = c;
		oldCellValue = oldV;
		newCellValue = newV;
		oldCellFormat = oldForm;
		newCellFormat = newForm;
		oldDecimalsToSet = oldDec;
		newDecimalsToSet = newDec;
	}
	//konstruktor za format_row
	ActionRecord(ActionType type, int r, vector<char> oldFormats, vector<char> newFormats) {
		actionType = type;
		row = r;
		oldRowFormats = oldFormats;
		newRowFormats = newFormats;
	}

	//konstruktor za format_column
	ActionRecord(ActionType type, int c, char oldGlobal, char newGlobal, map<int, char> oldFormats) {
		actionType = type;
		column = c;
		oldColumnFormats = oldFormats;
		oldGlobalColumnFormat = oldGlobal;
		newGlobalColumnFormat = newGlobal;
	}
};
#endif // !_ACTION_RECORD_STRUCT
