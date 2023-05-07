#ifndef _DATECELL_CLASS
#define _DATECELL_CLASS


#include "Cell.h"
#include "Errors.h"
using namespace std;
class DateCell : public Cell {
public:
	//DateCell(string startValue = "", int row = 0, ColumnLetters col = A) :Cell(startValue, row, col) {}
	DateCell(Table2* t, string input = "") :Cell(t, input) {
		if (!validInputForFormat(input)) throw CellInputError();
		setInputValue(input);
	}

	virtual string getFormattedValue() const override { return inputValue; }
	virtual char getFormat() override { return 'D'; }
	virtual void setInputValue(string val) override;
	virtual bool validInputForFormat(string input) override;
	static bool staticValidInput(string input);
private:
	static bool checkValidDate(int d, int m, int y);
	
};



#endif // !_DATECELL_CLASS
