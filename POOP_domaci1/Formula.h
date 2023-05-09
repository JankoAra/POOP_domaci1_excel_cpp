#ifndef _FORMULA_CLASS
#define _FORMULA_CLASS

#include "Table2.h"
#include <string>
#include <vector>
using namespace std;

class Formula {
public:
	//friend class Table;

	Formula(string exp, Table* t);

	string getExpression() const { return expression; }

	bool isInvalid() const { return invalidExpression; }

	double calculateFormula();

	string getFormulaValueAsString();
	
	
private:
	Table* table;
	string expression;
	static string getNextToken(string& input, int& index);
	static string getNextTokenPostfix(string& postfix, int& index);
	bool invalidExpression = false;
	vector<string> referencedCells;

	//menja reference na celije za vrednosti celija
	string dereferenceCells() const;

	//expression moze sadrzati samo brojeve i operande, bez referenci na celije, bez razmaka, bez =
	static string infixToPostfix(string expression);

	//izracunaj postfix izraz
	static double calculatePostfix(string postfix);
};



#endif // !_FORMULA_CLASS

