#ifndef _MENU_CLASS
#define _MENU_CLASS

#include <string>
#include "Table2.h"
#include "Parser.h"

using namespace std;


const int DEFAULT_LINE_LEN = 50;

class Menu {
public:
	Menu(Table2* table = nullptr) :currentTable(table) {}

	Table2* getTable() const { return currentTable; }
	void setTable(Table2* t) { currentTable = t; }

	//prikaz opcija menija
	static void displayStartMenu();
	static void displayMainMenu();
	static pair<char, int> chooseFormatMenu();

	
	//vraca redni broj izabrane opcije iz menija
	static int getMenuInputFromConsole();

	//vraca true ukoliko korisnik izabere opciju da zeli da sacuva trenutnu tabelu, inace false
	bool askToSave() const;

	//pita za ime fajla nad kojim se radi i na osnovu ekstenzije fajla vraca odgovarajuci parser
	Parser* makeParser() const;
	
	//ispisuje c karakter lineLen puta, pa novi red
	static void printLineOfChars(char c, int lineLen = DEFAULT_LINE_LEN);

	//ispisuje prosledjeni string kao gresku
	static void printErrorMsg(string errmsg);

	~Menu() {
		if (currentTable) delete currentTable;
		currentTable = nullptr;
	}
private:
	Table2* currentTable = nullptr;


};

#endif