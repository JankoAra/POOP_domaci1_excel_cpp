#include "CSVParser.h"
#include <fstream>
#include <iostream>
#include "Errors.h"
#include <sstream>
#include "TextCell.h"

void CSVParser::loadTable(Table* table){
	ifstream file(fileName);

	if (!file.is_open()) {
		throw FileNotOpen();
	}
	string line;
	int row = 0;
	while (getline(file, line)) {
		stringstream stream(line);
		string cellValue;
		int column = 0;
		while (getline(stream, cellValue, ',')) {
			Cell* cell = new TextCell(table, cellValue);
			table->setCell(row, column, cell);
			column++;
		}
		row++;
	}
	file.close();
}

void CSVParser::saveTable(Table* table){
	ofstream file(fileName);
	if (!file.is_open()) throw FileNotOpen();
	//racunanje najveceg reda
	int maxRowNumber = 0;
	for (auto& rowDesc : table->getData()) {
		maxRowNumber = max(maxRowNumber, rowDesc.first);
	}
	//upis u fajl vrednosti celija
	for (int i = 0; i < maxRowNumber; i++) {
		for (int j = 0; j < 26; j++) {
			Cell* cell = table->getCell(i, j);
			file << (cell?cell->getInputValue():"") << (j == 25 ? "\n" : ",");
		}
	}

	file.close();
	cout << "Sacuvan fajl: " << fileName << endl;
}
