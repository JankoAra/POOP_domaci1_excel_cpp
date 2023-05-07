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

void JSONParser::loadTable(Table2* table) {
	ifstream file(fileName);

	if (!file.is_open()) {
		throw FileNotOpen();
	}
	string line;

	//citanje globalnih formata za kolone
	string targetName = "\"globalColumnFormats\": [";
	while (getline(file, line)) {
		size_t pos = line.find(targetName);
		if (pos == string::npos) continue;
		size_t cutOffset = pos + targetName.length() - 1;
		string charArray = line.substr(cutOffset, line.length() - cutOffset - 1);
		vector<char> chars = getCharsFromJsonArray(charArray);
		for (int i = 0; i < 26; i++) table->getColumnFormats()[i] = chars[i];
		break;
	}

	//citanje globalnih decimala za kolone
	targetName = "\"globalColumnDecimals\": [";
	while (getline(file, line)) {
		size_t pos = line.find(targetName);
		if (pos == string::npos) continue;
		size_t cutOffset = pos + targetName.length() - 1;
		string charArray = line.substr(cutOffset, line.length() - cutOffset - 1);
		vector<int> ints = getIntsFromJsonArray(charArray);
		for (int i = 0; i < 26; i++) table->getColumnDecimals()[i] = ints[i];
		break;
	}

	//citanje celija
	while (true) {
		char c=' ';
		stringstream stream;
		while (c!='{') file.get(c);
		while (c != '}') {
			stream << c;
			file.get(c);
		}
		stream << c;
		JSONCell cellRecord = readJsonCell(stream.str());
		Cell* cell = table->createNewCellOfFormat(cellRecord.format, cellRecord.value, cellRecord.decimals);
		table->setCell(cellRecord.row, cellRecord.column, cell);
		stream.str("");
		file.get(c);
		if (c != ',') break;
	}

	file.close();
}

void JSONParser::saveTable(Table2* table) {
	ofstream file(fileName);
	if (!file.is_open()) throw FileNotOpen();
	file << "{\n";

	//cuvanje globalnih formata za kolone
	file << "\"globalColumnFormats\": [";
	//for_each(table->getColumnFormats(), table->getColumnFormats() + 26, [&file](char c) {file << "\"" << c << "\""; });
	char* columnFormats = table->getColumnFormats();
	for (int i = 0; i < 26; i++) {
		file << "\"" << columnFormats[i] << "\"" << (i == 25 ? "" : ",");
	}
	file << "],\n";

	//cuvanje globalmnih decimala za kolone
	file << "\"globalColumnDecimals\": [";
	//for_each(table->getColumnDecimals(), table->getColumnDecimals() + 26, [&file](int i) {file << i; });
	int* columnDecimals = table->getColumnDecimals();
	for (int i = 0; i < 26; i++) {
		file << "\"" << columnDecimals[i] << "\"" << (i == 25 ? "" : ",");
	}
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
	regex keyRegex("\"row\": (\\d+),");
	smatch match;
	if (!regex_search(jsonCellString, match, keyRegex)) throw JSONDataError();
	int row = stoi(match[1].str());

	//izvuci column
	keyRegex = "\"column\": (\\d+),";
	if (!regex_search(jsonCellString, match, keyRegex)) throw JSONDataError();
	int column = stoi(match[1].str());

	//izvuci format
	keyRegex = "\"format\": \"([DTN])\",";
	if (!regex_search(jsonCellString, match, keyRegex)) throw JSONDataError();
	char format = (match[1].str())[0];

	//izvuci decimals
	keyRegex = "\"decimals\": (-?\\d+)";
	if (!regex_search(jsonCellString, match, keyRegex)) throw JSONDataError();
	int decimals = stoi(match[1].str());

	//izvuci value
	keyRegex = "\"value\": \"([[:print:]]*)\",";
	if (!regex_search(jsonCellString, match, keyRegex)) throw JSONDataError();
	string value = match[1].str();
	return JSONCell(row, column, value, format, decimals);
}
