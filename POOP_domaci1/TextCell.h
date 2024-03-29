#ifndef _TEXTCELL_CLASS
#define _TEXTCELL_CLASS

#include "Cell.h"
using namespace std;

class TextCell : public Cell {
public:
	//TextCell(string startValue = "", int row = 0, ColumnLetters col = A):Cell(startValue, row, col){}
	TextCell(Table2* t, string input = "") :Cell(t, input) {}
	virtual string getFormattedValue() const override { return inputValue; }
	virtual char getFormat() override { return 'T'; }
	virtual bool validInputForFormat(string input) override { return true; }
};



#endif // !_TEXTCELL_CLASS

