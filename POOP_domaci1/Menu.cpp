#include "Menu.h"
#include "Errors.h"
#include "CSVParser.h"
#include "JSONParser.h"

#include <iostream>
#include <regex>
#include <vector>

void Menu::displayStartMenu() {
	vector<string> startMenuOptions;
	startMenuOptions.push_back("KREIRAJ NOVU TABELU");				//1
	startMenuOptions.push_back("UCITAJ POSTOJECU TABELU");			//2
	startMenuOptions.push_back("UGASI PROGRAM");					//0


	cout << "EXCEL by Janko" << endl;
	printLineOfChars('-');
	for (int i = 0; i < startMenuOptions.size(); i++) {
		int num = (i == startMenuOptions.size() - 1) ? 0 : i + 1;
		cout << num << ". " << startMenuOptions[i] << endl;
	}
	printLineOfChars('-');
	cout << "Unesite vas izbor: ";
}

void Menu::displayMainMenu() {
	vector<string> mainMenuOptions;
	mainMenuOptions.push_back("PROMENI VREDNOST POLJA");			//1
	mainMenuOptions.push_back("FORMATIRAJ POLJE/KOLONU/RED");		//2
	mainMenuOptions.push_back("UNDO");								//3
	mainMenuOptions.push_back("REDO");								//4
	mainMenuOptions.push_back("SAVE");								//5
	mainMenuOptions.push_back("SAVE AS");							//6
	mainMenuOptions.push_back("SAVE AND QUIT");						//7
	mainMenuOptions.push_back("RESETUJ TABELU");					//8
	mainMenuOptions.push_back("PROMENI SHEET");						//9
	mainMenuOptions.push_back("VRATI SE NA POCETNI MENI");			//10
	mainMenuOptions.push_back("UGASI PROGRAM");						//0

	printLineOfChars('-');
	cout << "GLAVNI MENI" << endl;
	for (int i = 0; i < mainMenuOptions.size(); i++) {
		int num = (i == mainMenuOptions.size() - 1) ? 0 : i + 1;
		cout << num << ". " << mainMenuOptions[i] << endl;
	}
	printLineOfChars('-');
	cout << "Unesite vas izbor: ";
}

pair<char, int> Menu::chooseFormatMenu() {
	vector<string> options;
	options.push_back("Text");
	options.push_back("Number");
	options.push_back("Date");
	cout << "Izaberite format:" << endl;
	for (int i = 0; i < options.size(); i++) {
		cout << i + 1 << ". " << options[i] << endl;
	}
	string choice;
	smatch match;
	regex pattern("^\\s*([1-3])\\s*$");
	int timeout = 3;
	do {
		getline(cin, choice);
		if (regex_search(choice, match, pattern)) break;
		cout << "Nepostojeca opcija, Pokusajte ponovo" << endl;
	} while (--timeout);
	int format = stoi(match[1].str());	//1=Text; 2=Number; 3=Date;
	char formatChar = format == 1 ? 'T' : (format == 2 ? 'N' : 'D');
	int decimals = 0;
	if (format == 2) {
		cout << "Unesite broj decimala: ";
		decimals = getMenuInputFromConsole();
		if (decimals < 0 || decimals>15) {
			printErrorMsg("Dozvoljeni broj decimala je 0-15. Koristice se default vrednost 2");
			decimals = 2;
		}
	}
	return make_pair(formatChar, decimals);
}


int Menu::getMenuInputFromConsole() {
	//cout << "Unesite vas izbor: ";
	regex pattern2("^\\s*\\d{1,2}\\s*$");	//cita jednocifrene i dvocifrene brojeve izmedju blanko znakova
	string line;
	getline(cin, line);

	//nepostojeca opcija
	if (!regex_match(line, pattern2)) return -1;

	regex blankRegex("\\s+");
	string res = regex_replace(line, blankRegex, "");	//brise blanko znakove
	int choice = stoi(res);
	return choice;
}

void Menu::printErrorMsg(string errmsg) {
	cout << "\033[1;31m" << errmsg << "!" << "\033[0m" << endl;
}

Parser* Menu::makeParser() const {
	cout << "Unesite ime/putanju fajla: ";
	string fName;
	regex pattern("^\\s*([^\\.]+)\\.([A-Za-z0-9]+)\\s*$");
	smatch match;
	getline(cin, fName);
	if (!regex_search(fName, match, pattern)) throw FileNameError();
	string fileFormat = match[2].str();
	if (fileFormat == "csv") {
		return new CSVParser(fName);
	}
	else if (fileFormat == "json") {
		return new JSONParser(fName);
	}
	return nullptr;
}

bool Menu::askToSave() const {
	cout << "Da li zelite da sacuvate tabelu? [d/n]";
	regex pattern("^[dn]$");
	string answer;
	int timeout = 3;
	do {
		getline(cin, answer);
		if (regex_match(answer, pattern)) {
			if (answer == "d") return true;
			return false;
		}
		cout << "Nepostojeca opcija. Pokusajte ponovo" << endl;
	} while (--timeout);
	cout << "Opcija nije uspesno izabrana. Tabela se nece cuvati" << endl;
	return false;
}

void Menu::printLineOfChars(char c, int lineLen) {
	string line(lineLen, c);
	cout << line << endl;
}
