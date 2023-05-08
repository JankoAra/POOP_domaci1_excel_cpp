#include "Menu.h"
#include "Table2.h"
#include "Errors.h"
#include <iostream>
using namespace std;

int main() {
	Menu menu;
	Table2* table = nullptr;
	bool done = false;
	Parser* loadParser = nullptr;
	Parser* saveParser = nullptr;
	while (!done) {
		menu.displayStartMenu();
		int choice = menu.getMenuInputFromConsole();
		if (choice < 0 || choice > 2) {
			menu.printErrorMsg("Nepostojeca opcija");
			continue;
		}
		if (choice == 0) {
			//prekid programa
			//oslobadjanje dinamicki zauzete memorije
			if (loadParser) { if (saveParser == loadParser) saveParser = nullptr; delete loadParser; }
			if (saveParser) delete saveParser;
			if (table) delete table;
			done = true;
			break;
		}
		//pravljenje prazne tabele
		if (table == nullptr) table = new Table2();
		if (choice == 2) {
			//ucitavanje tabele iz fajla
			try {
				if (loadParser) { delete loadParser; loadParser = nullptr; }
				loadParser = menu.makeParser();
				if (loadParser == nullptr) throw ParserNullPointer();
				loadParser->loadTable(table);
			}
			catch (exception& e) {
				printError(e);
				continue;
			}
		}
		bool innerDone = false;
		while (!done && !innerDone) {
			try {
				if (choice >= 0 && choice <= 10) table->printTable();	//ne stampa tabelu ako je bila pogresna opcija
				menu.displayMainMenu();
				choice = menu.getMenuInputFromConsole();
				switch (choice) {
				case 0:	//quit
					//ask to save
					if (menu.askToSave()) {
						//save
						if (saveParser) { delete saveParser; saveParser = nullptr; }
						if (loadParser) saveParser = loadParser;
						else saveParser = menu.makeParser();
						if (saveParser == nullptr) throw ParserNullPointer();
						saveParser->saveTable(table);
					}
					if (loadParser) { if (saveParser == loadParser) saveParser = nullptr; delete loadParser; loadParser = nullptr; }
					if (saveParser) { delete saveParser; saveParser = nullptr; }
					if (table) { delete table; table = nullptr; }
					done = true;
					break;
				case 1:		//promeni vrednost polja
					table->insertCellValue();
					break;
				case 2:		//formatiraj tabelu
					table->formatTable();
					break;
				case 3:		//undo
					table->undo();
					break;
				case 4:		//redo
					table->redo();
					break;
				case 5:		//save
					if (saveParser) { delete saveParser; saveParser = nullptr; }
					if (loadParser) saveParser = loadParser;
					else saveParser = menu.makeParser();
					if (saveParser == nullptr) throw ParserNullPointer();
					saveParser->saveTable(table);
					break;
				case 6:		//save as
					if (saveParser) { delete saveParser; saveParser = nullptr; }
					saveParser = menu.makeParser();
					if (saveParser == nullptr) throw ParserNullPointer();
					saveParser->saveTable(table);
					break;
				case 7:		//save and quit
					break;
				case 8:		//reset table
					break;
				case 9:		//change sheet
					break;
				case 10:	//back to start menu
					//ask to save
					if (menu.askToSave()) {
						//save
						if (saveParser) { delete saveParser; saveParser = nullptr; }
						if (loadParser) saveParser = loadParser;
						else saveParser = menu.makeParser();
						if (saveParser == nullptr) throw ParserNullPointer();
						saveParser->saveTable(table);
					}
					delete table;	//delete current table
					table = nullptr;
					innerDone = true;
					break;
				default:
					menu.printErrorMsg("Nepostojeca opcija");
				}
			}
			catch (exception& e) {
				printError(e);
			}
		}
	}
	cout << "Gasenje programa..." << endl;
	return 0;
}