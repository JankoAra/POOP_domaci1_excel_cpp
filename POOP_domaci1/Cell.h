#ifndef _CELL_CLASS
#define _CELL_CLASS

#include <string>

using namespace std;

class Table;

class Cell {
public:
	Cell(Table* t, string input = "") :inputValue(input), myTable(t) {}

	string getInputValue() const { return inputValue; }
	virtual void setInputValue(string val) { inputValue = val; }

	virtual char getFormat() = 0;
	virtual string getFormattedValue() const = 0;
	virtual bool validInputForFormat(string input) = 0;

	static string cellDescriptionInJson(int row, int column, Cell* cell);

	//odredjivanje formata celije u tabeli, row pocinje od 1
	//static char getFormat(int row, char col, Table* table);


	virtual ~Cell(){}
protected:
	string inputValue;
	Table* myTable;
};


#endif // !_CELL_CLASS
