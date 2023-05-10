#ifndef _NUMBERCELL_CLASS
#define _NUMBERCELL_CLASS

#include "Cell.h"
#include "Errors.h"
#include <vector>

using namespace std;

class NumberCell : public Cell {
public:
	NumberCell(Table* t, string input = "", int decimals = 2) :Cell(t, input), decimalSpaces(decimals) {
		if (!validInputForFormat(input)) throw CellInputError();
		setInputValue(input);
	}

	void setDecimalSpaces(int d) { decimalSpaces = d; }
	int getDecimalSpaces() const { return decimalSpaces; }

	//vraca numericku vrednost celije, poziva se iz Formula::dereferenceCells, garantovano se poziva za ispravne izraze
	double getNumberValue() const;

	//vraca string koji prestavlja broj sa odredjenim brojem decimala ili "ERROR" ako je izraz nevalidan; sluzi za ispis celije
	virtual string getFormattedValue() const override;

	virtual char getFormat() override { return 'N'; }

	//postavlja inputValue na val bez blanko znakova, ukoliko je val neki broj ili formula; u suprotnom baca CellInputError
	virtual void setInputValue(string val) override;

	//vraca true ako je input string koji predstavlja broj ili je formula, poziv je polimorfan
	virtual bool validInputForFormat(string input) override {
		return NumberCell::staticValidInput(input);
	}

	//vraca true ako je input string koji predstavlja broj ili je formula
	static bool staticValidInput(string input);

	static bool hasCircularRefrence(NumberCell* cell, vector<NumberCell*>& visited);

	vector<NumberCell*> getReferencedCells() const;
private:
	//string inputValue iz Cell
	//Table* myTable iz Cell
	int decimalSpaces;
};



#endif // !_NUMBERCELL_CLASS

