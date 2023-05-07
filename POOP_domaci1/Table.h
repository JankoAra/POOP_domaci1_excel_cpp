#ifndef _TABLE_CLASS
#define _TABLE_CLASS

#include <vector>
#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <iomanip>
#include "TextCell.h"
#include "NumberCell.h"
#include "DateCell.h"


using namespace std;
using RowDesc = pair<int, vector<Cell*>*>;
class Table{
public:
	Table();
	void printTable() const;
	void addRow(int rowNum);
	void insertValue();
	Cell* getCell(int r, int c);
	void setFormat();
private:
	vector<RowDesc>* matrix;

	int selectFormatMenu();
	int getRowIndex(int r) const;
};



#endif // !_TABLE_CLASS

