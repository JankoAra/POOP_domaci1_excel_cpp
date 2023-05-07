#ifndef _ERRORS_CLASS
#define _ERRORS_CLASS


#include <exception>
using namespace std;

void printError(exception& err);

class OptionNonExistent : public exception {
public:
	const char* what() const noexcept override {
		return "Opcija ne postoji.\tPokusajte unos ponovo";
	}
};

class UnknownState : public exception {
public:
	const char* what() const noexcept override {
		return "Nepoznato stanje!";
	}
};

class FormattingError : public exception {
public:
	const char* what() const noexcept override {
		return "Nemoguce promeniti format celije, jer vrednost celije ne odgovara novom formatu";
	}
};

class UnsuccessfulFormat : public exception {
public:
	const char* what() const noexcept override {
		return "Formatiranje nije uspesno izvrseno";
	}
};

class CellInputError : public exception {
public:
	const char* what() const noexcept override {
		return "Nova vrednost ne odgovara postojecem formatu celije";
	}
};

class InvalidDate : public exception {
public:
	const char* what() const noexcept override {
		return "Datum je nepostojeci";
	}
};

class ParserNullPointer : public exception {
public:
	const char* what() const noexcept override {
		return "Parser fajla nije uspesno kreiran";
	}
};

class FileNameError : public exception {
public:
	const char* what() const noexcept override {
		return "Ime fajla nije u odgovarajucem formatu";
	}
};

class FileNotOpen : public exception {
public:
	const char* what() const noexcept override {
		return "Fajl nije uspesno otvoren";
	}
};

class FormulaParameterNotNumber : public exception {
public:
	const char* what() const noexcept override {
		return "Parametar formule nije broj";
	}
};

class NotEnoughParentheses : public exception {
public:
	const char* what() const noexcept override {
		return "Izraz nije validan jer je broj otvorenih i zatvorenih zagrada nejednak";
	}
};

class ExpressionNotValid : public exception {
public:
	const char* what() const noexcept override {
		return "Izraz nije ispravan";
	}
};

class CellNotExists : public exception {
public:
	const char* what() const noexcept override {
		return "Celija ne postoji";
	}
};

class DivisionByZero : public exception {
public:
	const char* what() const noexcept override {
		return "Deljenje nulom nije dozvoljeno";
	}
};

class FormulaDestinationNotNumber : public exception {
public:
	const char* what() const noexcept override {
		return "Celija u koju se upisuje formula nije formatirana kao broj";
	}
};

class RecursiveReference : public exception {
public:
	const char* what() const noexcept override {
		return "Uocena je rekurzivna referenca u izrazu";
	}
};

class JSONDataError : public exception {
public:
	const char* what() const noexcept override {
		return "Greska u JSON fajlu";
	}
};



#endif // !_ERRORS_CLASS