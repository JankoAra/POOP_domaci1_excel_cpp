#ifndef _TEXTCELL_CLASS
#define _TEXTCELL_CLASS

#include "Cell.h"
using namespace std;

class TextCell : public Cell {
public:
	TextCell(Table* t, string input = "") :Cell(t, input) {}
	virtual string getFormattedValue() const override { return inputValue; }
	virtual char getFormat() override { return 'T'; }
	virtual bool validInputForFormat(string input) override { return true; }
};



#endif // !_TEXTCELL_CLASS

