#ifndef _FORMULA_CLASS
#define _FORMULA_CLASS

#include "Table2.h"
#include <string>
#include <vector>
#include "NumberCell.h"
using namespace std;

class Formula {
public:
	//friend class Table;

	Formula(string exp, Table* t);

	string getExpression() const { return expression; }

	bool isInvalid() const { return invalidExpression; }

	void setInvalid(bool invalid) { invalidExpression = invalid; }

	double calculateFormula();

	string getFormulaValueAsString();

	
	//menja reference na celije za vrednosti celija
	string dereferenceCells();

private:
	Table* table;
	string expression;
	static string getNextToken(string& input, int& index);
	static string getNextTokenPostfix(string& postfix, int& index);
	bool invalidExpression = false;

	//menja reference na celije za vrednosti celija
	//string dereferenceCells() const;

	

	//expression moze sadrzati samo brojeve i operatore, bez referenci na celije, bez razmaka, bez =
	static string infixToPostfix(string expression);

	//izracunaj postfix izraz
	static double calculatePostfix(string postfix);
};



#endif // !_FORMULA_CLASS