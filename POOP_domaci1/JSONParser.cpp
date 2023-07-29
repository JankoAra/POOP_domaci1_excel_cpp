#include "JSONParser.h"
#include <iostream>
#include <fstream>
#include "Errors.h"
#include <algorithm>
#include <map>
#include "Cell.h"
#include <cctype>
#include <sstream>
#include <regex>

void JSONParser::loadTable(Table* table) {
	ifstream file(fileName);

	if (!file.is_open()) {
		throw FileNotOpen();
	}
	string line;
	stringstream stream;
	stream << file.rdbuf();
	string JSONString = stream.str();
	stream.str("");

	//citanje globalnih formata za kolone
	size_t pos = JSONString.find("\"globalColumnFormats\"");
	if (pos == string::npos) throw JSONDataError();
	size_t openBracket = JSONString.find("[", pos);
	size_t closedBracket = JSONString.find("]", openBracket);
	string formatsArray = JSONString.substr(openBracket, closedBracket - openBracket + 1);
	vector<char> chars = getCharsFromJsonArray(formatsArray);
	for (int i = 0; i < 26; i++) table->getColumnFormats()[i] = chars[i];

	//citanje globalnih decimala za kolone
	pos = JSONString.find("\"globalColumnDecimals\"");
	if (pos == string::npos) throw JSONDataError();
	openBracket = JSONString.find("[", pos);
	closedBracket = JSONString.find("]", openBracket);
	string decimalsArray = JSONString.substr(openBracket, closedBracket - openBracket + 1);
	vector<int> ints = getIntsFromJsonArray(decimalsArray);
	for (int i = 0; i < 26; i++) table->getColumnDecimals()[i] = ints[i];

	//citanje celija
	pos = JSONString.find("\"cells\"");
	if (pos == string::npos) throw JSONDataError();
	openBracket = JSONString.find("[", pos);
	closedBracket = JSONString.find("]", openBracket);
	string cellsArrayString = JSONString.substr(openBracket, closedBracket - openBracket + 1);
	while (true) {
		size_t openCurly = cellsArrayString.find("{");
		size_t closedCurly = cellsArrayString.find("}");
		if (openCurly == string::npos || closedCurly == string::npos) break;
		string jsonCellString = cellsArrayString.substr(openCurly, closedCurly - openCurly + 1);
		cellsArrayString.erase(openCurly, closedCurly - openCurly + 1);
		JSONCell cellRecord = readJsonCell(jsonCellString);
		Cell* cell = table->createNewCellOfFormat(cellRecord.format, cellRecord.value, cellRecord.decimals);
		table->setCell(cellRecord.row, cellRecord.column, cell);
	}

	file.close();
}

void JSONParser::saveTable(Table* table) {
	ofstream file(fileName);
	if (!file.is_open()) throw FileNotOpen();
	file << "{\n";

	//cuvanje globalnih formata za kolone
	file << "\"globalColumnFormats\": [";
	char* columnFormats = table->getColumnFormats();
	for_each(columnFormats, columnFormats + 26, [&file, &columnFormats](char& format) {
		file << "\"" << format << "\"" << (&format == columnFormats + 25 ? "" : ",");
		});
	file << "],\n";

	//cuvanje globalmnih decimala za kolone
	file << "\"globalColumnDecimals\": [";
	int* columnDecimals = table->getColumnDecimals();
	for_each(columnDecimals, columnDecimals + 26, [&file, &columnDecimals](int& decimal) {
		file << "\"" << decimal << "\"" << (&decimal == columnDecimals + 25 ? "" : ",");
		});
	file << "],\n";

	//cuvanje niza celija
	file << "\"cells\": [";
	map<int, map<int, Cell*>> cells = table->getData();
	int numOfIterations = 0;
	int currentIteration = 0;
	for (auto& rowDesc : cells) {
		numOfIterations += rowDesc.second.size();
	}
	for (auto& rowDesc : cells) {
		for (auto& colDesc : rowDesc.second) {
			file << Cell::cellDescriptionInJson(rowDesc.first, colDesc.first, colDesc.second);
			if (++currentIteration < numOfIterations) {
				file << ",";
			}
			file << "\n";
		}
	}
	file << "]";

	file << "}";
	file.close();
	cout << "Sacuvan fajl: " << fileName << endl;
}

vector<char> JSONParser::getCharsFromJsonArray(string jsonArray) const {
	vector<char> chars;
	for (char c : jsonArray) {
		if (isalpha(c)) chars.push_back(c);
	}
	return chars;
}

vector<int> JSONParser::getIntsFromJsonArray(string jsonArray) const {
	vector<int> ints;
	bool numberStarted = false;
	stringstream numberStream;
	for (char c : jsonArray) {
		if (isdigit(c)) {
			numberStarted = true;
			numberStream << c;
		}
		else {
			if (numberStarted) ints.push_back(stoi(numberStream.str()));
			numberStarted = false;
			numberStream.str("");
		}
	}
	return ints;
}

JSONCell JSONParser::readJsonCell(string jsonCellString) const {
	//izvuci row
	regex keyRegex("\"row\":(\\d+)");
	smatch match;
	if (!regex_search(jsonCellString, match, keyRegex)) throw JSONDataError();
	int row = stoi(match[1].str());

	//izvuci column
	keyRegex = "\"column\":(\\d+)";
	if (!regex_search(jsonCellString, match, keyRegex)) throw JSONDataError();
	int column = stoi(match[1].str());

	//izvuci format
	keyRegex = "\"format\":\"([DTN])\"";
	if (!regex_search(jsonCellString, match, keyRegex)) throw JSONDataError();
	char format = (match[1].str())[0];

	//izvuci decimals
	keyRegex = "\"decimals\":(-?\\d+)";
	if (!regex_search(jsonCellString, match, keyRegex)) throw JSONDataError();
	int decimals = stoi(match[1].str());

	//izvuci value
	/*keyRegex = "\"value\":\"([[:print:]]*)\"";
	if (!regex_search(jsonCellString, match, keyRegex)) throw JSONDataError();
	string value = match[1].str();*/

	size_t openValue = jsonCellString.find("\"value\":\"");
	if (openValue == string::npos) throw JSONDataError();
	size_t offset = string("\"value\":\"").length();
	size_t closedValue = jsonCellString.find("\"", offset + openValue);
	if (closedValue == string::npos) throw JSONDataError();
	string value = jsonCellString.substr(openValue + offset, closedValue - (openValue + offset));

	return JSONCell(row, column, value, format, decimals);
}
