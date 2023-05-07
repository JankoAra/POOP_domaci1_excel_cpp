#include "Errors.h"
#include <iostream>

void printError(exception& err) {
	cout << "\033[1;31m" << err.what() << "!" << "\033[0m" << endl;
	/*const int lineLen = 50;
	string line(lineLen, '!');
	cout << line << endl << err.what() << endl << line << endl;*/
}