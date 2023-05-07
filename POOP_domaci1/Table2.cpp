#include "Table2.h"
#include "TextCell.h"
#include "DateCell.h"
#include "NumberCell.h"
#include "Errors.h"
#include "Formula.h"
#include "Menu.h"
#include "Action.h"
#include "Cell.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <numeric>
#include <regex>
#include <cctype>
#include <algorithm>

Table2::Table2() {
	fill(columnFormats, columnFormats + 26, 'T');
	fill(columnDecimals, columnDecimals + 26, 2);
}

Cell* Table2::getCell(int row, int column) const {
	auto rowIterator = cells.find(row);
	if (rowIterator == cells.end()) return nullptr;		// ne postoji ni jedna celija u zadatom redu
	auto columnIterator = rowIterator->second.find(column);
	if (columnIterator == rowIterator->second.end()) return nullptr;	//ne postoji celija u zadatom redu i koloni
	return columnIterator->second;
}

void Table2::setCell(int row, int column, Cell* newCell) {
	if (newCell == nullptr) {
		//ako postavljamo nullptr, brisemo celiju
		auto rowIterator = cells.find(row);
		if (rowIterator != cells.end()) {
			auto columnIterator = rowIterator->second.find(column);
			if (columnIterator != rowIterator->second.end()) {
				delete columnIterator->second;
				rowIterator->second.erase(columnIterator);
			}
		}
	}
	else {
		if (cells[row][column]) delete cells[row][column];
		cells[row][column] = newCell;
	}
}

void Table2::setCell(int row, ColumnLetters column, Cell* newCell) {
	if (row < 1) throw CellNotExists();
	setCell(row - 1, (int)column, newCell);
}

Cell* Table2::getCell(int row, ColumnLetters column) const {
	if (row < 1) throw CellNotExists();
	return getCell(row - 1, (int)column);
}

void Table2::setCell(int row, char column, Cell* newCell) {
	if (row < 1) throw CellNotExists();
	column = toupper(column);
	if (column < 'A' || column>'Z') return;
	setCell(row - 1, column - 'A', newCell);
}

Cell* Table2::getCell(int row, char column) const {
	if (row < 1) throw CellNotExists();
	column = toupper(column);
	if (column < 'A' || column>'Z') return nullptr;
	return getCell(row - 1, column - 'A');
}

void Table2::setCell(string cellName, Cell* cell) {
	if (cellName.length() < 2) throw CellNotExists();
	if (!isalpha(cellName[0])) throw CellNotExists();
	if (!isdigit(cellName[1])) throw CellNotExists();
	char column = toupper(cellName[0]);
	int row = stoi(cellName.substr(1, cellName.length() - 1));
	return setCell(row, column, cell);
}

Cell* Table2::getCell(string cellName) const {
	if (cellName.length() < 2) throw CellNotExists();
	if (!isalpha(cellName[0])) throw CellNotExists();
	if (!isdigit(cellName[1])) throw CellNotExists();
	char column = toupper(cellName[0]);
	int row = stoi(cellName.substr(1, cellName.length() - 1));
	return getCell(row, column);
}

char Table2::getCellFormat(int row, int column) const {
	Cell* cell = getCell(row, column);
	if (cell) return cell->getFormat();
	return columnFormats[column];
}

char Table2::getCellFormat(int row, char column) const {
	return getCellFormat(row - 1, column - 65);
}

void Table2::insertCellValue() {
	//Izbor celije
	cout << "Unesite celiju koju menjate(npr. C8 ili c8)" << endl;
	regex cellNamePattern("^\\s*([A-Za-z])([1-9]\\d{0,5})\\s*$");	//ogranicene kolone A-Z(a-z) i redovi 1-999.999
	string line;
	smatch match;
	getline(cin, line);
	if (!regex_search(line, match, cellNamePattern)) throw CellNotExists();	//ime celije nije ispravno
	int row = stoi(match[2].str());	//brojanje od 1
	char column = toupper((match[1].str())[0]);	//karakter A-Z
	Cell* oldCell = getCell(row, column);
	Cell* newCell = nullptr;

	//Unos vrednosti
	cout << "Unesite vrednost: ";
	getline(cin, line);

	//Ispitivanje da li je uneta formula
	regex formulaPattern("^\\s*=");
	bool isFormula = false;
	if (regex_search(line, formulaPattern)) {
		//formula
		isFormula = true;
		try {
			//formulu mozemo upisati samo u celiju formatiranu kao broj
			if (Cell::getFormat(row, column, this) != 'N') throw FormulaDestinationNotNumber();
		}
		catch (FormulaDestinationNotNumber& err) { printError(err); return; }
	}

	try {
		char format = Cell::getFormat(row, column, this);
		int decimalsToSet = 0;
		if (format == 'N') decimalsToSet = oldCell ? ((NumberCell*)oldCell)->getDecimalSpaces() : columnDecimals[column - 65];
		newCell = createNewCellOfFormat(format, line, decimalsToSet);

		//napravi ActionRecord
		string oldCellValue = oldCell ? oldCell->getInputValue() : "";
		undoStack.push(Action(row - 1, column - 65, oldCellValue, line, format, decimalsToSet, this));


		setCell(row, column, newCell);	//brise oldCell

		//praznjenje redo steka
		clearStack(redoStack);
	}
	catch (exception& e) {
		printError(e);
	}
}


void Table2::printTable() const {
	// Racunaj maksimalne sirine kolona i najveci redni broj reda
	vector<int> columnWidths(27, 1);
	int maxRowNumber = 0;
	for (auto& rowDesc : cells) {
		ostringstream stream;
		stream << rowDesc.first + 1;
		maxRowNumber = max(maxRowNumber, rowDesc.first + 1);
		columnWidths[0] = max(columnWidths[0], (int)((stream.str()).length()));
		for (auto& column : rowDesc.second) {
			Cell* cell = column.second;
			if (cell != nullptr) {
				int columnWidth = cell->getFormattedValue().length();
				columnWidths[column.first + 1] = max(columnWidths[column.first + 1], columnWidth);
			}
		}
	}
	//Napravi separator redova
	int separatorLength = accumulate(columnWidths.begin(), columnWidths.end(), 28);
	string rowSeparator(separatorLength, '-');
	rowSeparator += "\n";
	cout << rowSeparator;

	//Ispisi prvi red sa imenima kolona
	cout << "|";
	for (int i = 0; i < 27; i++) {
		string columnName;
		if (i == 0) columnName = "";
		else columnName = 'A' + i - 1;
		cout << left << setw(columnWidths[i]) << columnName << (i == 26 ? "|\n" : "|");
	}
	cout << rowSeparator;

	//Ispisi ostale redove sa celijama
	for (int i = 0; i < maxRowNumber; i++) {
		cout << "|";
		for (int j = 0; j < 27; j++) {
			string cellContent;
			ostringstream stream;
			if (j == 0) {
				stream << i + 1;
				cellContent = stream.str();
			}
			else {
				Cell* cell = getCell(i, j - 1);
				if (cell == nullptr) cellContent = "";
				else cellContent = cell->getFormattedValue();
			}
			if (cellContent == "ERROR") cellContent = "\033[1;31mERROR\033[0m";
			cout << left << setw(columnWidths[j]) << cellContent << (j == 26 ? "|\n" : "|");
		}
		cout << rowSeparator;
	}

}

void Table2::undo(bool redoFlag) {
	if (undoStack.empty()) return;
	Action record = undoStack.top();
	undoStack.pop();
	if (redoFlag == false) {
		//ako trenutno radimo undo
		redoStack.push(Action::reverseAction(record));
	}
	else {
		//ako trenutno radimo redo
		undoStack.push(Action::reverseAction(record));
	}
	map<char, int> intFormat = { {'T',1},{'N',2},{'D',3} };
	if (record.row >= 0 && record.column >= 0) {
		//promena vrednosti ili formata celije
		Cell* newCell = createNewCellOfFormat(record.oldRowFormats[record.column], record.oldCellValue, record.oldRowDecimals[record.column]);
		setCell(record.row, record.column, newCell);
	}
	else if (record.row >= 0 && record.column < 0) {
		//row format
		for (int i = 0; i < 26; i++) {
			Cell* newCell = createNewCellOfFormat(record.oldRowFormats[i], \
				getCell(record.row, i)->getInputValue(), \
				record.oldRowDecimals[i]);
			setCell(record.row, i, newCell);
		}
	}
	else if (record.row < 0 && record.column >= 0) {
		//column format
		columnFormats[record.column] = record.oldGlobalFormats[record.column];
		columnDecimals[record.column] = record.oldGlobalDecimals[record.column];
		map<int, Cell*> allCellsInColumn = cellsInColumn(record.column);
		for (auto& rowCell : allCellsInColumn) {
			Cell* newCell = createNewCellOfFormat(record.oldColumnFormats[record.column], \
				getCell(rowCell.first, record.column)->getInputValue(), \
				record.oldColumnDecimals[record.column]);
			setCell(rowCell.first, record.column, newCell);
		}
	}
	else {
		Menu::printErrorMsg("Record nije ispravan");
	}
}

void Table2::redo() {
	if (redoStack.empty()) {
		cout << "Redo stek prazan" << endl;
		return;
	}
	Action action = redoStack.top();
	redoStack.pop();
	//stavljamo action na undo stek, jer undo i redo rade iste stvari
	undoStack.push(action);
	undo(true);	//undo(true) je redo
}

void Table2::changeCellFormat(int row, char column, char formatToSet, int decimals) {
	//decimals parametar bitan samo ako menjamo format u number
	Cell* oldCell = getCell(row, column);
	string cellValue = oldCell != nullptr ? oldCell->getInputValue() : "";

	if (!newFormatFitsInput(formatToSet, cellValue)) throw FormattingError();

	Cell* newCell = createNewCellOfFormat(formatToSet, cellValue, decimals);	//ne bi trebalo da baca izuzetke

	//napravi ActionRecord
	undoStack.push(Action(row - 1, column - 65, cellValue, cellValue, formatToSet, decimals, this));

	setCell(row, column, newCell);	// brise oldCell iz memorije
}

void Table2::changeRowFormat(int row, char formatToSet, int decimals) {
	//provera da li novi format odgovara svim postojecim vrednostima u redu
	for (auto& rowDesc : cells[row - 1]) {
		Cell* cell = getCell(row - 1, rowDesc.first);	//oldCell ne bi trebalo da moze da bude nullptr
		try {
			if (!newFormatFitsInput(formatToSet, cell->getInputValue())) throw FormattingError();
		}
		catch (FormattingError& err) {
			Menu::printErrorMsg("Nemoguce promeniti format svih celija u redu jer neka od " \
				"njih sadrzi vrednost koja ne odgovara novom formatu");
			throw UnsuccessfulFormat();
		}

	}

	//napravi ActionRecord
	undoStack.push(Action(row - 1, -1, "", "", formatToSet, decimals, this));

	//ako nema koflikta formata i sadrzaja, promeni format svih celija u redu
	for (int i = 0; i < 26; i++) {
		Cell* oldCell = getCell(row - 1, i);
		string cellValue = oldCell != nullptr ? oldCell->getInputValue() : "";
		Cell* newCell = createNewCellOfFormat(formatToSet, cellValue, decimals);	//ne bi trebalo da baca izuzetke
		setCell(row - 1, i, newCell);	//brise sve oldCell
	}
}

void Table2::changeColumnFormat(char column, char formatToSet, int decimals) {
	//provera da li novi format odgovara svim vrednostima u koloni
	for (auto& rowCell : cellsInColumn(column)) {
		Cell* cell = rowCell.second;
		try {
			if (!newFormatFitsInput(formatToSet, cell->getInputValue())) throw FormattingError();
		}
		catch (FormattingError& err) {
			Menu::printErrorMsg("Nemoguce promeniti format svih celija u koloni jer neka od " \
				"njih sadrzi vrednost koja ne odgovara novom formatu");
			throw UnsuccessfulFormat();
		}
	}

	//napravi ActionRecord
	undoStack.push(Action(-1, column - 65, "", "", formatToSet, decimals, this));

	//promeni globalni format i broj decimala za kolonu
	columnFormats[column - 65] = formatToSet;
	columnDecimals[column - 65] = decimals;

	//promeni formate svih celija u koloni
	for (auto& rowDesc : cells) {
		Cell* oldCell = getCell(rowDesc.first + 1, column);
		if (oldCell == nullptr) continue;
		string cellValue = oldCell->getInputValue();
		Cell* newCell = createNewCellOfFormat(formatToSet, cellValue, decimals);
		setCell(rowDesc.first + 1, column, newCell);	//brise sve oldCell
	}
}

void Table2::formatTable() {
	cout << "Unesite celiju, red ili kolonu koju menjate(npr. C8 ili c8; A ili a; 3)" << endl;
	regex cellNamePattern("^\\s*([A-Za-z])([1-9]\\d{0,5})\\s*$");
	regex rowPattern("^\\s*([1-9]\\d{0,5})\\s*$");
	regex columnPattern("^\\s*([A-Za-z])\\s*$");
	bool formatCell = false;
	bool formatRow = false;
	bool formatColumn = false;
	smatch match;
	string line;
	getline(cin, line);
	if (regex_search(line, match, cellNamePattern)) formatCell = true;
	else if (regex_search(line, match, rowPattern)) formatRow = true;
	else if (regex_search(line, match, columnPattern)) formatColumn = true;
	else {
		Menu::printErrorMsg("Unos nije ispravan. Povratak u glavni meni");
		return;
	}
	auto pair = Menu::chooseFormatMenu();
	char formatToSet = pair.first;
	int decimalsToSet = pair.second;
	if (formatCell) {
		//formatiraj jednu celiju
		int row = stoi(match[2].str());
		char column = toupper((match[1].str())[0]);
		changeCellFormat(row, column, formatToSet, decimalsToSet);
	}
	else if (formatRow) {
		//formatiraj red
		int row = stoi(match[1].str());
		changeRowFormat(row, formatToSet, decimalsToSet);
	}
	else {
		//formatiraj kolonu
		char column = toupper((match[1].str())[0]);
		changeColumnFormat(column, formatToSet, decimalsToSet);
	}
	// ocisti redo stack
	clearStack(redoStack);
}

Cell* Table2::createNewCellOfFormat(int format, string input, int decimals) {
	switch (format) {
	case 1:
		return new TextCell(this, input);
	case 2:
		return new NumberCell(this, input, decimals);
	case 3:
		return new DateCell(this, input);
	}
	return nullptr;
}

Cell* Table2::createNewCellOfFormat(char format, string input, int decimals) {
	int f = format == 'T' ? 1 : (format == 'N' ? 2 : 3);
	return createNewCellOfFormat(f, input, decimals);
}

bool Table2::newFormatFitsInput(int format, string input) const {
	switch (format) {
	case 1:
		return true;
	case 2:
		if (NumberCell::staticValidInput(input)) return true;
		break;
	case 3:
		if (DateCell::staticValidInput(input)) return true;
	}
	return false;
}

bool Table2::newFormatFitsInput(char format, string input) const {
	int f = format == 'T' ? 1 : (format == 'N' ? 2 : 3);
	return newFormatFitsInput(f, input);
}

map<int, Cell*> Table2::cellsInColumn(int column) {
	map<int, Cell*> res;
	for (auto& rowDesc : cells) {
		for (auto& columnDesc : rowDesc.second) {
			if (columnDesc.first == column) res[rowDesc.first] = columnDesc.second;
		}
	}
	return res;
}

map<int, Cell*> Table2::cellsInColumn(char column) {
	return cellsInColumn(toupper(column) - 65);
}

void Table2::clearStack(stack<Action>& stack) {
	while (!stack.empty()) {
		stack.pop();
	}
}
