#include "Action.h"
#include "Cell.h"
#include "NumberCell.h"

#include "Table.h"

Action::Action(int r, int c, string oldVal, string newVal, char newFormat, int newDecimals, Table* table) {
	row = r;
	column = c;
	oldCellValue = oldVal;
	newCellValue = newVal;
	for (int i = 0; i < 26; i++) {
		oldGlobalFormats[i] = (table->getColumnFormats())[i];
		newGlobalFormats[i] = newFormat;
		oldGlobalDecimals[i] = (table->getColumnDecimals())[i];
		newGlobalDecimals[i] = newDecimals;
		if (r >= 0) {
			oldRowFormats[i] = table->getCellFormat(r, i);
			newRowFormats[i] = newFormat;
			Cell* oldCell = table->getCell(r, i);
			if (oldRowFormats[i] == 'N' && oldCell != nullptr) oldRowDecimals[i] = ((NumberCell*)table->getCell(r, i))->getDecimalSpaces();
			else oldRowDecimals[i] = -1;
			newRowDecimals[i] = newDecimals;
		}
	}
	if (c >= 0) {
		std::map<int, Cell*> allCellsInColumn = table->cellsInColumn(c);
		for (auto& rowCell : allCellsInColumn) {
			oldColumnFormats[rowCell.first] = rowCell.second->getFormat();
			newColumnFormats[rowCell.first] = newFormat;
			if (rowCell.second->getFormat() == 'N') oldColumnDecimals[rowCell.first] = ((NumberCell*)rowCell.second)->getDecimalSpaces();
			else oldColumnDecimals[rowCell.first] = -1;
			newColumnDecimals[rowCell.first] = newDecimals;
		}
	}

}

Action Action::reverseAction(Action& action) {
	Action reaction;
	reaction.row = action.row;
	reaction.column = action.column;
	reaction.oldCellValue = action.newCellValue;
	reaction.newCellValue = action.oldCellValue;
	for (int i = 0; i < 26; i++) {
		reaction.oldGlobalFormats[i] = action.newGlobalFormats[i];
		reaction.newGlobalFormats[i] = action.oldGlobalFormats[i];
		reaction.oldGlobalDecimals[i] = action.newGlobalDecimals[i];
		reaction.newGlobalDecimals[i] = action.oldGlobalDecimals[i];
		reaction.oldRowFormats[i] = action.newRowFormats[i];
		reaction.newRowFormats[i] = action.oldRowFormats[i];
		reaction.oldRowDecimals[i] = action.newRowDecimals[i];
		reaction.newRowDecimals[i] = action.oldRowDecimals[i];
	}
	reaction.oldColumnFormats = action.newColumnFormats;
	reaction.newColumnFormats = action.oldColumnFormats;
	reaction.oldColumnDecimals = action.newColumnDecimals;
	reaction.newColumnDecimals = action.oldColumnDecimals;
	return reaction;
}
