#include "Cell.h"
#include "Table2.h"
#include <sstream>
#include "NumberCell.h"

string Cell::cellDescriptionInJson(int row, int column, Cell* cell) {
	stringstream stream;
	stream << "{\n";

	stream << "\"row\": " << row << ",\n";
	stream << "\"column\": " << column << ",\n";
	stream << "\"value\": \"" << cell->getInputValue() << "\",\n";
	stream << "\"format\": \"" << cell->getFormat() << "\",\n";
	stream << "\"decimals\": " << (cell->getFormat() == 'N' ? ((NumberCell*)cell)->getDecimalSpaces() : -1) << "\n";

	stream << "}";
	return stream.str();
}

//row se broji od 1, col je veliko slovo
char Cell::getFormat(int row, char col, Table2* table) {
	Cell* cell = table->getCell(row, col);
	if (cell) return cell->getFormat();
	return table->getColumnFormats()[col - 65];
}
