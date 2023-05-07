#ifndef _CELL_CLASS
#define _CELL_CLASS

#include <string>

using namespace std;

class Table2;

class Cell {
public:
	Cell(Table2* t, string input = "") :inputValue(input), myTable(t) {}

	string getInputValue() const { return inputValue; }
	virtual void setInputValue(string val) { inputValue = val; }

	virtual char getFormat() = 0;
	virtual string getFormattedValue() const = 0;
	virtual bool validInputForFormat(string input) = 0;

	static string cellDescriptionInJson(int row, int column, Cell* cell);

	//odredjivanje formata celije u tabeli, row pocinje od 1
	static char getFormat(int row, char col, Table2* table);
protected:
	string inputValue;
	Table2* myTable;
	/*
public:
	enum ColumnLetters { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z };
	Cell(string startValue = "", int row = 0, ColumnLetters col = A) {
		this->inputValue = startValue;
		this->row = row;
		this->column = col;
	}
	string getInputValue() const { return inputValue; }
	virtual void setInputValue(string val) { inputValue = val; }
	virtual char getFormat() = 0;
	virtual string getFormattedValue() const = 0;
protected:
	string inputValue;
	int row;
	ColumnLetters column;

	*/
};


#endif // !_CELL_CLASS
