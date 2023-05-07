//#include "Table.h"
//
//Table::Table() {
//	matrix = new vector<RowDesc>();
//}
//
//void Table::printTable() const {
//	if (!matrix->size()) return;
//	int maxRow = (*matrix)[matrix->size() - 1].first;
//	auto iter = matrix->begin();
//	int columnsToDisplay = 6;
//	int cellWidth = 15;
//	for (int i = 0; i < maxRow; i++) {
//		if (iter._Unwrapped()->first == i + 1) {
//			int count = 0;
//			for (Cell* cell : *(iter._Unwrapped()->second)) {
//				cout << setw(cellWidth) << (cell ? cell->getFormattedValue() : "") << "|";
//				if (++count == columnsToDisplay)break;
//			}
//			cout << endl;
//			iter++;
//		}
//		else {
//			for (int count = 0; count < columnsToDisplay; count++) {
//				cout << setw(cellWidth) << "" << "|";
//			}
//			cout << endl;
//		}
//	}
//}
//
//void Table::addRow(int rowNum) {
//	bool found = any_of(matrix->begin(), matrix->end(), [rowNum](pair<int, vector<Cell*>*>& elem) {return elem.first == rowNum; });
//	if (found) return;
//	vector<Cell*>* newRow = new vector<Cell*>(26);
//	RowDesc newPair = make_pair(rowNum, newRow);
//	matrix->push_back(newPair);
//	sort(matrix->begin(), matrix->end(), [](pair<int, vector<Cell*>*>& a, pair<int, vector<Cell*>*>& b) {return a.first < b.first; });
//}
//
//void Table::insertValue() {
//	cout << "Unesite celiju koju menjate(npr. C8 ili c8)" << endl;
//	regex cellNamePattern("^\\s*[A-Za-z]\\d{1,6}\\s*$");
//	string line;
//	getline(cin, line);
//	if (!regex_match(line, cellNamePattern)) {
//		cout << "Nepostojeca celija" << endl;
//		return;
//	}
//	regex blanks("\\s+");
//	string cellName = regex_replace(line, blanks, "");
//	cellName[0] = toupper(cellName[0]);
//	int row = stoi(cellName.substr(1, cellName.length() - 1));
//	int column = cellName[0] - 65;	//A-->0, B-->1, ..., Z-->25
//	cout << "Unesite vrednost: ";
//	getline(cin, line);
//	addRow(row);
//	Cell* cell = getCell(row, column);
//	if (cell == nullptr) {
//		//ako celija jos ne postoji
//		int rIndex = getRowIndex(row);
//		(*(*matrix)[rIndex].second)[column] = new TextCell(line, row, (Cell::ColumnLetters)column);
//		return;
//	}
//	//ako je celija vec napravljena, moze imati neki format
//	try {
//		cell->setInputValue(line);
//	}
//	catch (exception& e) {
//		printError(e);
//	}
//
//	//cout << "U celiju " << cellName << " upisano:" << line << endl;
//}
//
//Cell* Table::getCell(int r, int c) {
//	int rIndex = getRowIndex(r);
//	if (rIndex == -1) return nullptr;
//	return (*(*matrix)[rIndex].second)[c];
//}
//
//void Table::setFormat() {
//	cout << "Unesite celiju, red ili kolonu koju menjate(npr. C8 ili c8; A ili a; 3)" << endl;
//	regex cellNamePattern("^[\\s]*([A-Za-z])(\\d{1,6})[\\s]*$");
//	regex rowPattern("^\\s*(\\d{1,6})\\s*$");
//	regex columnPattern("^\\s*([A-Za-z])\\s*$");
//	smatch match;
//	string line;
//	getline(cin, line);
//	int format;
//	if (regex_search(line, match, rowPattern)) {
//		//formatiraj ceo red
//		format = selectFormatMenu();
//	}
//	else if (regex_search(line, match, columnPattern)) {
//		//formatiraj celu kolonu
//		format = selectFormatMenu();
//	}
//	else if (regex_search(line, match, cellNamePattern)) {
//		//formatiraj jednu celiju
//		format = selectFormatMenu();
//		int row = stoi(match[2].str());
//		int column = toupper((match[1].str())[0]) - 65;
//		addRow(row);
//		Cell* cell = getCell(row, column);
//		regex number("^\\s*(\\d+)\\s*$");
//		string decimals;
//		int decimalSpaces;
//		//smatch match;
//		if (cell == nullptr) {
//			int rIndex = getRowIndex(row);
//			switch (format) {
//			case 1:
//				(*(*matrix)[rIndex].second)[column] = new TextCell("", row, (Cell::ColumnLetters)column);
//				break;
//			case 2:
//				cout << "Unesite broj decimala: ";
//				do {
//					getline(cin, decimals);
//					if (regex_search(decimals, match, number)) break;
//					cout << "Neispravan unos. Pokusajte ponovo" << endl;
//				} while (true);
//				decimalSpaces = stoi(match[1].str());
//				
//				(*(*matrix)[rIndex].second)[column] = new NumberCell("", row, (Cell::ColumnLetters)column, decimalSpaces);
//				break;
//			case 3:
//				(*(*matrix)[rIndex].second)[column] = new DateCell("", row, (Cell::ColumnLetters)column);
//				break;
//			}
//		}
//		else {
//			try {
//				int rIndex = getRowIndex(row);
//				switch (format) {
//				case 1:
//					(*(*matrix)[rIndex].second)[column] = new TextCell(cell->getInputValue(), row, (Cell::ColumnLetters)column);
//					break;
//				case 2:
//					cout << "Unesite broj decimala: ";
//					do {
//						getline(cin, decimals);
//						if (regex_search(decimals, match, number)) break;
//						cout << "Neispravan unos. Pokusajte ponovo" << endl;
//					} while (true);
//					decimalSpaces = stoi(match[1].str());
//
//					(*(*matrix)[rIndex].second)[column] = new NumberCell(cell->getInputValue(), row, (Cell::ColumnLetters)column, decimalSpaces);
//					(*(*matrix)[rIndex].second)[column]->setInputValue(cell->getInputValue());
//					break;
//				case 3:
//					(*(*matrix)[rIndex].second)[column] = new DateCell(cell->getInputValue(), row, (Cell::ColumnLetters)column);
//					(*(*matrix)[rIndex].second)[column]->setInputValue(cell->getInputValue());
//					break;
//				}
//				delete cell;
//			}
//			catch (exception& e) {
//				printError(e);
//			}
//		}
//	}
//	else {
//		cout << "Unos nije ispravan" << endl;
//		return;
//	}
//
//	/*
//	regex blanks("\\s+");
//	string cellName = regex_replace(line, blanks, "");
//	cellName[0] = toupper(cellName[0]);
//	int row = stoi(cellName.substr(1, cellName.length() - 1));
//	int column = cellName[0] - 65;	//A-->0, B-->1, ..., Z-->25
//	cout << "Unesite vrednost: ";
//	getline(cin, line);
//	addRow(row);
//	Cell* cell = getCell(row, column);
//	*/
//}
//
//int Table::selectFormatMenu() {
//	vector<string> options;
//	options.push_back("Text");
//	options.push_back("Number");
//	options.push_back("Date");
//	cout << "Izaberite format:" << endl;
//	for (int i = 0; i < options.size(); i++) {
//		cout << i + 1 << ". " << options[i] << endl;
//	}
//	string choice;
//	smatch match;
//	regex pattern("^\\s*([1-3])\\s*$");
//	do {
//		getline(cin, choice);
//		if (regex_search(choice, match, pattern)) break;
//		cout << "Nepostojeca opcija, Pokusajte ponovo" << endl;
//	} while (!regex_search(choice, match, pattern));
//	return stoi(match[1].str());
//}
//
//int Table::getRowIndex(int r) const {
//	auto found = find_if(matrix->begin(), matrix->end(), [r](pair<int, vector<Cell*>*>& elem) {return elem.first == r; });
//	if (found == matrix->end()) return -1;
//	int ind = distance(matrix->begin(), found);
//	return ind;
//}
