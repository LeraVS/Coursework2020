#include "Utils.h"

void utils::enterInInterval(int & number, int min, int max) {
	while (true) {
		getNumber(number);
		if (number >= min && number <= max)
			return;

		std::cout << "Ошибка! Повторите ввод.\n";
	}
}

std::string utils::inputString() {
	std::string val;

	while (true) {
		try {
			std::getline(std::cin, val);
			if (val.find_first_not_of(' ') == std::string::npos) throw new std::runtime_error("Пустая строка. Повторите ввод: \n");
			else break;
		} catch (std::runtime_error* e) {
			std::cout << e->what();
		}
	}
	return val;
}

void utils::inputCount(int & count) {
	while (true) {
		try {
			getNumber<int>(count);
			if (count <= 0) throw new std::out_of_range("Отрицательное число вместо положительного.\n");
			else break;
		} catch (std::out_of_range * e) {
			std::cout << e->what();
		} catch (std::length_error e) {
			std::cout << "Введено недопустимо большое число.\n";
		}

		std::cout << "Повторите ввод: ";
	}
}

void utils::inputCount(float & count) {
	while (true) {
		try {
			getNumber<float>(count);
			if (count <= 0) throw new std::out_of_range("Отрицательное число вместо положительного.\n");
			else break;
		} catch (std::out_of_range * e) {
			std::cout << e->what();
		} catch (std::length_error e) {
			std::cout << "Введено недопустимо большое число.\n";
		}

		std::cout << "Повторите ввод: ";
	}
}

tm utils::inputDate() {
	tm date;
	int number;

	std::cout << "Год: ";
	enterInInterval(number, 1900, 3000);
	date.tm_year = number;

	std::cout << "Месяц: ";
	enterInInterval(number, 1, 12);

	date.tm_mon = number - 1;

	std::cout << "День: ";
	if (date.tm_mon == 1) enterInInterval(number, 1, 29);
	else if (date.tm_mon == 3 || date.tm_mon == 5 || date.tm_mon == 8 || date.tm_mon == 10) enterInInterval(number, 1, 30);
	else enterInInterval(number, 1, 31);
	date.tm_mday = number;

	return date;
}

std::string utils::inputPassword() {
	char password[100];
	int length = 0;

	while (true) {
		password[length] = _getwch();

		if (password[length] == '\r')
			break;
		else if (password[length] == '\b') {
			if (length > 0) {
				std::cout << "\b \b";
				length--;
			}
		} else {
			std::cout << "*";
			length++;
		}
	}

	password[length] = '\0';

	std::cout << "\n";
	return (std::string)password;
}

void utils::outputDate(tm & date) {
	if (date.tm_mday < 10)
		std::cout << std::setw(4) << '0' << date.tm_mday << ".";
	else std::cout << std::setw(4) << date.tm_mday << ".";
	if (date.tm_mon < 9)
		std::cout << '0' << date.tm_mon + 1 << "." << date.tm_year;
	else std::cout << date.tm_mon + 1 << "." << date.tm_year;
}

bool utils::YesOrNo(std::string str) {
	while (true) {
		std::cout << str;
		switch (_getwch()) {
		case 'y': return true;
		case 'н': return true;
		case 'n': return false;
		case 'т': return false;
		default:
			std::cout << "Неверный номер действия. Повторите попытку снова.\n";
			pause();
		}
	}
}

int utils::selectingId(int size) {
	int id;
	std::cout << "Введите ID товара: \n";
	enterInInterval(id, 1, size);
	return id;
}

void utils::pause() {
	std::cout << "\n";
	system("pause");
}

void utils::clearScreen() { system("cls"); }

std::string utils::getString(SOCKET& connection) {
	char* buffer = new char[1000];

	recv(connection, buffer, 1000 * sizeof(char), 0);
	std::string value = std::string(buffer);

	delete[]buffer;
	return value;
}

int utils::getInt(SOCKET& connection) {
	char* buffer = new char[1000];

	recv(connection, buffer, 1000 * sizeof(char), 0);
	int value = atoi(buffer);

	delete[]buffer;
	return value;
}

void utils::sendInt(SOCKET& connection, const int & value) {
	char* buffer = new char[1000];

	_itoa_s(value, buffer, 1000, 10);
	send(connection, buffer, 1000 * sizeof(char), 0);

	delete[]buffer;
}

void utils::sendString(SOCKET& connection, const std::string & value) {
	char buf[1000];
	strcpy_s(buf, value.c_str());
	send(connection, buf, 1000 * sizeof(char), 0);
}
