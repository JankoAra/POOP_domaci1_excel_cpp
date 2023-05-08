#ifndef _JSONPARSER_CLASS
#define _JSONPARSER_CLASS

#include "Parser.h"
#include <vector>

struct JSONCell {
	char format;
	int row, column, decimals;
	string value;

	JSONCell(int r, int c, string val, char f, int dec) {
		row = r;
		column = c;
		format = f;
		decimals = dec;
		value = val;
	}
};

class JSONParser :public Parser {
public:
	JSONParser(string fName) :Parser(fName) {}

	virtual void loadTable(Table* table) override;
	virtual void saveTable(Table* table) override;


	vector<char> getCharsFromJsonArray(string jsonArray) const;
	vector<int> getIntsFromJsonArray(string jsonArray) const;
	JSONCell readJsonCell(string jsonCellString) const;

private:
	

	//string fileName iz Parser
};



#endif // !_JSONPARSER_CLASS

