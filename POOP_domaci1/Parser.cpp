#include "Parser.h"
#include <iostream>
#include <regex>
#include "Menu.h"
int Parser::setFileNameAndChooseFileFormat(){
	cout << "Unesite ime/putanju fajla: ";
	string fName;
	regex pattern("^\\s*([^\\.]+)\\.([A-Za-z0-9]+)\\s*$");
	smatch match;
	getline(cin, fName);
	if (!regex_search(fName, match, pattern)) {
		Menu::printErrorMsg("Neispravno ime fajla");
		return 0;
	}
	string fileFormat = match[2].str();
	int ret = 0;
	if (fileFormat == "csv") {
		ret = 1;
		fileName = fName;
	}
	else if (fileFormat == "json") {
		ret = 2;
		fileName = fName;
	}
	return ret;
}
