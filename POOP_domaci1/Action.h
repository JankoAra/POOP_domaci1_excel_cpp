#ifndef _ACTION_STRUCT
#define _ACTION_STRUCT

#include <vector>
#include <map>
#include <string>

class Table;

using namespace std;

struct Action {
	int row, column;
	string oldCellValue, newCellValue;
	char oldGlobalFormats[26], newGlobalFormats[26];
	int oldGlobalDecimals[26], newGlobalDecimals[26];
	char oldRowFormats[26], newRowFormats[26];
	int oldRowDecimals[26], newRowDecimals[26];
	map<int, char> oldColumnFormats, newColumnFormats;
	map<int, int> oldColumnDecimals, newColumnDecimals;

	//default konstruktor
	Action() {}

	//konstruktor za sve slucajeve, r i c se broje od nule, imaju vrednost -1 ako nisu relevantni za slucaj
	Action(int r, int c, string oldVal, string newVal, char newFormat, int newDecimals, Table* table);

	static Action reverseAction(Action& action);
};

#endif
