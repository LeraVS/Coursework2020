#include "Utils.h"

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

void utils::encrypt(char* message) {
	for (int i = 0; i < strlen(message); i++)
		message[i] = message[i] ^ KEY;
}

void utils::decrypt(char* message) {
	encrypt(message);
}

std::string utils::getTag(std::string tag, std::string readFrom) {
	std::regex regEx(tag + "=\"([^\"]*)\"");
	std::smatch match;
	std::regex_search(readFrom, match, regEx);
	if (match.size() > 1)
		return match.str(1);
	return "";

}

std::string utils::useGeek(int value) {
	std::stringstream geek;
	std::string line;
	geek << value;
	geek >> line;
	return line;
}

int utils::useGeek(std::string line) {
	std::stringstream geek;
	int value;
	geek << line;
	geek >> value;
	return value;
}

int utils::compareDates(const tm &date1, const tm &date2) {
	if (date1.tm_year < date2.tm_year)
		return -1;
	if (date1.tm_year > date2.tm_year)
		return 1;

	if (date1.tm_mon < date2.tm_mon)
		return -1;
	if (date1.tm_mon > date2.tm_mon)
		return 1;

	if (date1.tm_mday < date2.tm_mday)
		return -1;
	if (date1.tm_mday > date2.tm_mday)
		return 1;

	return 0;
}

bool operator<(const tm & date1, const tm & date2) {
	return utils::compareDates(date1, date2) == -1;
}

bool operator==(const tm & date1, const tm & date2) {
	return utils::compareDates(date1, date2) == 0;
}
