#pragma once
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <exception>
#include <windows.h>

namespace utils {
	template<
		typename T,
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
		void getNumber(T &number) {
		while (!(std::cin >> number) || (std::cin.peek() != '\n')) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cout << "Ошибка! Повторите ввод: " << "\n";
		}
		std::cin.ignore();
	}

	void enterInInterval(int & number, int min, int max);
	std::string inputString();
	void inputCount(int & count);
	void inputCount(float & count);
	tm inputDate();
	std::string inputPassword();

	void outputDate(tm& date);

	bool YesOrNo(std::string str);
	int selectingId(int size);
	void pause();
	void clearScreen();

	std::string getString(SOCKET& connection);
	int getInt(SOCKET& connection);
	void sendInt(SOCKET& connection, const int &value);
	void sendString(SOCKET& connection, const std::string &value);

}
