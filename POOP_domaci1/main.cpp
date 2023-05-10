#include "Menu.h"
#include "Table2.h"
#include "Errors.h"
#include <iostream>
#include "Parser.h"
#include <functional>
#include <map>

using namespace std;
using Function = function<void()>;
using FunctionMap = map<int, Function>;

int main() {
	Menu menu;
	Table* table = nullptr;
	bool done = false;
	bool innerDone = false;
	Parser* parser = nullptr;
	auto deleteParser = [&parser]() {
		delete parser;
		parser = nullptr;
	};
	auto deleteTable = [&table]() {
		delete table;
		table = nullptr;
	};
	auto save = [&parser, &table, &menu]() {
		if (parser == nullptr) parser = menu.makeParser();
		if (parser == nullptr) throw ParserNullPointer();
		parser->saveTable(table);
	};
	auto saveAs = [&parser, &deleteParser, &table, &menu]() {
		deleteParser();
		parser = menu.makeParser();
		if (parser == nullptr) throw ParserNullPointer();
		parser->saveTable(table);
	};

	FunctionMap mainMenuFunctions;
	mainMenuFunctions[1] = [&table]() {table->insertCellValue(); };
	mainMenuFunctions[2] = [&table]() {table->formatTable(); };
	mainMenuFunctions[3] = [&table]() {table->undo(); };
	mainMenuFunctions[4] = [&table]() {table->redo(); };
	mainMenuFunctions[5] = [&save]() {save(); };
	mainMenuFunctions[6] = [&saveAs]() {saveAs(); };
	mainMenuFunctions[7] = [&deleteTable, &menu, &save, &innerDone, &deleteParser]() {
		if (menu.askToSave()) {
			save();
		}
		deleteParser();
		deleteTable();
		innerDone = true;
	};
	mainMenuFunctions[0] = [&deleteTable, &menu, &save, &done, &deleteParser]() {
		if (menu.askToSave()) {
			save();
		}
		done = true;
	};
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
			done = true;
			break;
		}
		//pravljenje prazne tabele
		if (table == nullptr) table = new Table();
		if (choice == 2) {
			//ucitavanje tabele iz fajla
			try {
				deleteParser();
				parser = menu.makeParser();
				if (parser == nullptr) throw ParserNullPointer();
				parser->loadTable(table);
			}
			catch (exception& e) {
				printErrorMsg(e);
				continue;
			}
		}
		innerDone = false;
		while (!done && !innerDone) {
			try {
				if (choice >= 0 && choice <= 10) table->printTable();	//ne stampa tabelu ako je bila pogresna opcija
				menu.displayMainMenu();
				choice = menu.getMenuInputFromConsole();
				if (choice >= mainMenuFunctions.size()) {
					menu.printErrorMsg("Nepostojeca opcija");
					continue;
				}
				mainMenuFunctions[choice]();
			}
			catch (exception& e) {
				printErrorMsg(e);
			}
		}
	}
	deleteParser();
	deleteTable();
	cout << "Gasenje programa..." << endl;
	return 0;
}