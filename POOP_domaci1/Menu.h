#ifndef _MENU_CLASS
#define _MENU_CLASS

#include <string>

using namespace std;

class Parser;

class Menu {
public:

	//prikaz opcija menija
	static void displayStartMenu();
	static void displayMainMenu();
	static pair<char, int> chooseFormatMenu();

	
	//vraca redni broj izabrane opcije iz menija(ogranicen izmedju 0 i 99)
	static int getMenuInputFromConsole();

	//vraca true ukoliko korisnik izabere opciju da zeli da sacuva trenutnu tabelu, inace false
	static bool askToSave();

	//pita za ime fajla nad kojim se radi i na osnovu ekstenzije fajla vraca odgovarajuci parser
	static Parser* makeParser();

	//ispisuje prosledjeni string kao gresku crvenim slovima
	static void printErrorMsg(string errmsg);

	static const int DEFAULT_LINE_LEN = 50;
};

#endif