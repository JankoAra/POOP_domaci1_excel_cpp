#ifndef _TABLE_CLASS
#define _TABLE_CLASS

#include <map>
#include <vector>
#include <stack>
#include <string>
#include "Action.h"

using namespace std;

class Cell;


class Table {
public:
	friend class Formula;
	Table();
	//brojanje redova i kolona pocinje od 0
	Cell* getCell(int row, int column) const;
	void setCell(int row, int column, Cell* newCell);

	//brojanje redova pocinje od 1, kolone su char A-Z
	void setCell(int row, char column, Cell* newCell);
	Cell* getCell(int row, char column) const;

	//referenciranje pomocu imena poput A3, B5, d9
	void setCell(string cellName, Cell* cell);
	Cell* getCell(string cellName) const;

	//row i column se broje od 0
	char getCellFormat(int row, int column) const;

	//row od 1, char A-Z
	char getCellFormat(int row, char column) const;

	map<int, map<int, Cell*>> getData() const { return cells; }

	char* getColumnFormats() { return columnFormats; }

	int* getColumnDecimals() { return columnDecimals; }

	//vraca samo celije koje postoje u memoriji
	map<int, Cell*> cellsInColumn(int column);
	map<int, Cell*> cellsInColumn(char column);

	//upis u celiju
	void insertCellValue();


	void printTable() const;

	void undo(bool redoFlag = false);

	void redo();

	//row se broji od 1, column je char, formatToSet je 'T' / 'N' / 'D'
	void changeCellFormat(int row, char column, char formatToSet, int decimals);

	//row se broji od 1
	void changeRowFormat(int row, char formatToSet, int decimals);

	//char je A-Z
	void changeColumnFormat(char column, char formatToSet, int decimals);

	//formatiranje jedne celije, kolone ili reda
	void formatTable();

	Cell* createNewCellOfFormat(int format, string input = "", int decimals = 2);
	Cell* createNewCellOfFormat(char format, string input = "", int decimals = 2);
	bool newFormatFitsInput(int format, string input) const;
	bool newFormatFitsInput(char format, string input) const;

	~Table();

private:
	map<int, map<int, Cell*>> cells;	//prvi kljuc je red, drugi kljuc je kolona, vrednost je pokazivac na celiju
	char columnFormats[26];		// formati kolona
	int columnDecimals[26];		//default broj decimala u koloni, ako je formatirana kao number

	stack<Action> undoStack;
	stack<Action> redoStack;



	void clearStack(stack<Action>& stack);
};



#endif // !_TABLE_CLASS