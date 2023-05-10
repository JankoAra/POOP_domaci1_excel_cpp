#ifndef _PARSER_CLASS
#define _PARSER_CLASS
#include <string>
#include "Table.h"
using namespace std;

class Parser{
public:
	Parser(string fName):fileName(fName){}
	virtual void loadTable(Table* table) = 0;
	virtual void saveTable(Table* table) = 0;
	int setFileNameAndChooseFileFormat();
	string getFileName() { return fileName; }

	//pita za ime fajla nad kojim se radi i na osnovu ekstenzije fajla vraca odgovarajuci parser
	static Parser* makeParser();
protected:
	string fileName;
};


#endif // !_PARSER_CLASS


