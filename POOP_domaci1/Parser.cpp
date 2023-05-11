#include "Parser.h"
#include <iostream>
#include <regex>
#include "Errors.h"
#include "CSVParser.h"
#include "JSONParser.h"

Parser* Parser::makeParser(){
	cout << "Unesite ime/putanju fajla (obavezna ekstenzija .csv ili .json): ";
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
	printErrorMsg("Fajl sa datim formatom nije podrzan!");
	return nullptr;
}
