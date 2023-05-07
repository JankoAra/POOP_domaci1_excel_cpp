#include "Menu.h"
#include "Errors.h"
#include <iostream>
using namespace std;

int main() {
	Menu menu;
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
			if (menu.getTable()) delete menu.getTable();
			done = true;
			break;
		}
		//pravljenje prazne tabele
		if (menu.getTable() == nullptr) menu.setTable(new Table2());
		if (choice == 2) {
			//ucitavanje tabele iz fajla
			try {
				if (loadParser) { delete loadParser; loadParser = nullptr; }
				loadParser = menu.makeParser();
				if (loadParser == nullptr) throw ParserNullPointer();
				loadParser->loadTable(menu.getTable());
			}
			catch (exception& e) {
				printError(e);
				continue;
			}
		}
		bool innerDone = false;
		while (!done && !innerDone) {
			try {
				if (choice >= 0 && choice <= 10) menu.getTable()->printTable();	//ne stampa tabelu ako je bila pogresna opcija
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
						saveParser->saveTable(menu.getTable());
					}
					if (loadParser) { if (saveParser == loadParser) saveParser = nullptr; delete loadParser; loadParser = nullptr; }
					if (saveParser) { delete saveParser; saveParser = nullptr; }
					if (menu.getTable()) { delete menu.getTable(); menu.setTable(nullptr); }
					done = true;
					break;
				case 1:		//promeni vrednost polja
					menu.getTable()->insertCellValue();
					break;
				case 2:		//formatiraj tabelu
					menu.getTable()->formatTable();
					break;
				case 3:		//undo
					menu.getTable()->undo();
					break;
				case 4:		//redo
					menu.getTable()->redo();
					break;
				case 5:		//save
					if (saveParser) { delete saveParser; saveParser = nullptr; }
					if (loadParser) saveParser = loadParser;
					else saveParser = menu.makeParser();
					if (saveParser == nullptr) throw ParserNullPointer();
					saveParser->saveTable(menu.getTable());
					break;
				case 6:		//save as
					if (saveParser) { delete saveParser; saveParser = nullptr; }
					saveParser = menu.makeParser();
					if (saveParser == nullptr) throw ParserNullPointer();
					saveParser->saveTable(menu.getTable());
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
						saveParser->saveTable(menu.getTable());
					}
					delete menu.getTable();	//delete current table
					menu.setTable(nullptr);
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