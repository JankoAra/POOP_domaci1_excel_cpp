#ifndef _CSVPARSER_CLASS
#define _CSVPARSER_CLASS


#include "Parser.h"
class CSVParser : public Parser {
public:
	CSVParser(string fName):Parser(fName){}

	virtual void loadTable(Table2* table) override;
	virtual void saveTable(Table2* table) override;
};



#endif // !_CSVPARSER_CLASS

