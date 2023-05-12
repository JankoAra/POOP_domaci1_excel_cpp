#include "Cell.h"
#include "Table.h"
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