#pragma once
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>

static int KEY = 7337;

namespace utils {
	std::string getString(SOCKET& connection);
	int getInt(SOCKET& connection);

	void sendInt(SOCKET& connection, const int &value);
	void sendString(SOCKET& connection, const std::string &value);

	void encrypt(char * message);
	void decrypt(char * message);

	std::string getTag(std::string tag, std::string readFrom);
	std::string useGeek(int value);
	int useGeek(std::string line);

	int compareDates(const tm &date1, const tm &date2);
}

bool operator<(const tm &date1, const tm &date2);
bool operator==(const tm &date1, const tm &date2);