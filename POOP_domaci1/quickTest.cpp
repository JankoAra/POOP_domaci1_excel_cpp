#include "Table2.h"
#include "TextCell.h"
#include "NumberCell.h"
#include <iostream>
#include "Formula.h"
#include "JSONParser.h"
int main2() {
	//try {
	//	Table table;
	//	TextCell* tc1 = new TextCell(&table,"janko");
	//	NumberCell* tc2 = new NumberCell(&table, "5.5", 4);
	//	TextCell* tc3 = new TextCell(&table, "trt");
	//	TextCell* tc4 = new TextCell(&table, "nesto dugacko sa spejsom");
	//	NumberCell* tc5 = new NumberCell(&table, ".2", 2);
	//	NumberCell* tc6 = new NumberCell(&table, "20", 1);
	//	NumberCell* tc7 = new NumberCell(&table, "100", 0);
	//	table.setCell(1, Table::A, tc1);
	//	table.setCell(3, Table::E, tc2);
	//	table.setCell(3, Table::A, tc3);
	//	table.setCell(5, Table::B, tc4);
	//	table.setCell(5, Table::W, tc5);
	//	table.setCell(10, Table::A, tc6);
	//	table.setCell(10, Table::E, tc7);
	//	//table.format();
	//	table.printTable();
	//	string exp = "= w5 + 1 + 3-E3-4/1*aa2";
	//	//string expression = "-((+1.5-+-3)-4.)*2/5";
	//	Formula f(exp, &table);
	//	//cout << "\033[1;31m" << f.getFormulaValueAsString() << "\033[0m" << endl;
	//	cout << f.getFormulaValueAsString() << endl;
	//	/*string deref = f.dereferenceCells();
	//	string post = Formula::infixToPostfix(deref);
	//	cout << expression << endl << deref << endl << post << endl << Formula::calculatePostfix(post);*/
	//}
	//catch (exception& e) {
	//	printErrorMsg(e);
	//}

	JSONParser parser("fajl.json");
	string jsonCellString = "{\n\"row\": 0,\n\"column\": 0,\n\"value\": \"janko\",\n\"format\": \"T\",\n\"decimals\": -1\n}";
	JSONCell cell = parser.readJsonCell(jsonCellString);
	return 0;
}