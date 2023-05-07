#ifndef _PARSER_CLASS
#define _PARSER_CLASS
#include <string>
#include "Table2.h"
using namespace std;


class Parser{
public:
	Parser(string fName):fileName(fName){}
	virtual void loadTable(Table2* table) = 0;
	virtual void saveTable(Table2* table) = 0;
	int setFileNameAndChooseFileFormat();
	string getFileName() { return fileName; }
protected:
	string fileName;
};


#endif // !_PARSER_CLASS


