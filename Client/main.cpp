#include "Utils.h"
#pragma warning(disable:4996)

void Function(SOCKET& connection);

bool authorizationAdmin(SOCKET& connection);
bool authorizationUser(SOCKET& connection);

void enterMenu(SOCKET& connection);
void adminMenu(SOCKET& connection);
void userMenu(SOCKET& connection);
void requestToQueueMenu(SOCKET& connection);

void addUser(SOCKET& connection);
void addDocuments(SOCKET& connection);

bool removeUser(SOCKET& connection);

void registeringUserInQueue(SOCKET& connection);
void printingQueue(SOCKET& connection);
void usersPassThroughCustoms(SOCKET& connection);

void personalAccount(SOCKET& connection);
void sendingRequest(SOCKET& connection);
void requestVerification(SOCKET& connection);

void editExpertsScores(SOCKET& connection);
void editData(SOCKET& connection);

void outputUsers(SOCKET& connection);
void outputFirstExpertsScores(SOCKET& connection);
void outputSecondExpertsScores(SOCKET& connection);
void outputDocuments(SOCKET& connection, std::vector <int> documents = std::vector <int>());

void selectingId(SOCKET& connection);

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		std::cout << "Error\n";
		exit(1);
	}

	SOCKADDR_IN addr;
	int addrSize = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1280);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	SOCKET connection = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(connection, (sockaddr*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}

	Function(connection);

	closesocket(connection);
	WSACleanup();
}

void Function(SOCKET& connection) {
	int menu = 0;
	bool check = true;
	while (check) {
		utils::clearScreen();
		std::cout << "������� ����\n\n"
			<< "1. ����.\n"
			<< "2. �����������.\n"
			<< "3. �����.\n"
			<< "��� �����: ";
		utils::enterInInterval(menu, 1, 3);
		utils::sendInt(connection, menu);
		switch (menu) {
		case 1: enterMenu(connection); break;
		case 2: addUser(connection); break;
		case 3: check = false; break;
		}
	}
}

bool authorizationAdmin(SOCKET& connection) {
	utils::clearScreen();
	std::string login, password;
	int i = 3;
	if (!utils::getInt(connection)) {
		std::cout << "������� �����: ";
		login = utils::inputString();
		utils::sendString(connection, login);
		std::cout << "������� ������: ";
		password = utils::inputString();
		utils::sendString(connection, password);
		return true;
	}
	while (i) {
		std::cout << "������� �����: ";
		login = utils::inputString();
		utils::sendString(connection, login);
		std::cout << "������� ������: ";
		password = utils::inputPassword();
		utils::sendString(connection, password);
		if (utils::getInt(connection))
			return true;
		std::cout << "�������� ����. �������� " << i - 1 << " �������.\n";
		i--;
		if (i == 0) break;
		if (!utils::YesOrNo("������� ���������? y/n ")) {
			utils::sendInt(connection, 0);
			return false;
		}
		utils::sendInt(connection, 1);
		std::cout << "\n";
	}
	if (i == 0)
		std::cout << "��������� ���������� �������.\n";
	utils::pause();
	return false;
}

bool authorizationUser(SOCKET& connection) {
	utils::clearScreen();
	std::string login, password;
	int i = 3;
	while (i) {
		std::cout << "������� �����: ";
		login = utils::inputString();
		utils::sendString(connection, login);
		std::cout << "������� ������: ";
		password = utils::inputPassword();
		utils::sendString(connection, password);
		if (utils::getInt(connection))
			return true;
		std::cout << "�������� ����. �������� " << i - 1 << " �������.\n";
		i--;
		if (i == 0) break;
		if (!utils::YesOrNo("������� ���������? y/n ")) {
			utils::sendInt(connection, 0);
			return false;
		}
		utils::sendInt(connection, 1);
		std::cout << "\n";
	}
	if (i == 0)
		std::cout << "��������� ���������� �������.\n";
	utils::pause();
	return false;
}

void enterMenu(SOCKET& connection) {
	int menu = 0;
	while (true) {
		utils::clearScreen();
		std::cout << "�������� ������� ��� �����:\n\n"
			<< "1. �������������.\n"
			<< "2. ������������.\n"
			<< "3. ��������� �����.\n"
			<< "��� �����: \n";
		utils::enterInInterval(menu, 1, 3);
		utils::sendInt(connection, menu);
		switch (menu) {
		case 1: if (!authorizationAdmin(connection)) return;
				else adminMenu(connection); break;
		case 2: if (!authorizationUser(connection)) return;
				else userMenu(connection); break;
		case 3: return;
		}
	}
}

void adminMenu(SOCKET& connection) {
	int menu = 0;
	while (true) {
		utils::clearScreen();
		std::cout << "���� ��������������:\n\n"
			<< "1. �������� ������ � �������;\n"
			<< "2. �������� �������;\n"
			<< "3. ������ �������;\n"
			<< "4. ������� ������������ ����� �������;\n"
			<< "5. ��������� ���������� ������;\n"
			<< "6. ��������� �����.\n"
			<< "��� �����: ";
		utils::enterInInterval(menu, 1, 6);
		utils::sendInt(connection, menu);
		utils::clearScreen();
		switch (menu) {
		case 1: registeringUserInQueue(connection); break;
		case 2: if (utils::getInt(connection)) std::cout << "������� �����.\n";
				else outputUsers(connection);
			break;
		case 3: printingQueue(connection); break;
		case 4: usersPassThroughCustoms(connection); break;
		case 5: editExpertsScores(connection); break;
		case 6: return;
		}
		utils::pause();
	}
}

void userMenu(SOCKET& connection) {
	int menu = 0;
	while (true) {
		utils::clearScreen();
		std::cout << "���� ������������:\n\n"
			<< "1. ������ �������;\n"
			<< "2. �������� ����������;\n"
			<< "3. ��������� ������ � ����������� �������;\n"
			<< "4. ������ � �������;\n"
			<< "5. �������� ������ � �������;\n"
			<< "6. �������� ��������;\n"
			<< "7. ��������� �����.\n"
			<< "��� �����: ";
		utils::enterInInterval(menu, 1, 7);
		utils::sendInt(connection, menu);
		utils::clearScreen();
		switch (menu) {
		case 1: personalAccount(connection); break;
		case 2: addDocuments(connection); break;
		case 3: editData(connection); break;
		case 4: requestToQueueMenu(connection); break;
		case 5: requestVerification(connection); break;
		case 6: if (removeUser(connection)) return;
				else break;
		case 7: return;
		}
		utils::pause();
	}
}

void requestToQueueMenu(SOCKET& connection) {
	int menu = 0;
	std::cout << "�������� ��������:\n\n"
		<< "1. �������� �������;\n"
		<< "2. �������� �������;\n"
		<< "3. ��������� �����.\n"
		<< "��� �����: ";
	utils::enterInInterval(menu, 1, 3);
	utils::sendInt(connection, menu);
	utils::clearScreen();
	switch (menu) {
	case 1: sendingRequest(connection); break;
	case 2: requestVerification(connection); break;
	case 3: return;
	}
}

void addUser(SOCKET& connection) {
	utils::clearScreen();
	std::string login, password;

	std::string surname, name;
	int ID;
	tm dayOfBirth;
	std::cout << "�������: ";
	surname = utils::inputString();
	utils::sendString(connection, surname);
	std::cout << "���: ";
	name = utils::inputString();
	utils::sendString(connection, name);
	std::cout << "����������������� �����: ";
	utils::inputCount(ID);
	utils::sendInt(connection, ID);
	std::cout << "���� ��������: \n";
	dayOfBirth = utils::inputDate();
	utils::sendInt(connection, dayOfBirth.tm_mday);
	utils::sendInt(connection, dayOfBirth.tm_mon);
	utils::sendInt(connection, dayOfBirth.tm_year);
	std::cout << "\n";
	int answer = 0;
	std::cout << "������ ����������� �������:\n"
		<< "1. ������;\n"
		<< "2. ������;\n"
		<< "3. �������;\n"
		<< "4. ����;\n";
	utils::enterInInterval(answer, 1, 4);
	utils::sendInt(connection, answer);

	if (answer == 4) {
		std::cout << "\n����������� ����:\n"
			<< "1. ��������������� ����������;\n"
			<< "2. ����������;\n"
			<< "3. ����������;\n";
		utils::enterInInterval(answer, 1, 3);
		utils::sendInt(connection, answer);
	}
	std::cout << "\n";
	std::cout << "������� �����: ";
	while (true) {
		login = utils::inputString();
		utils::sendString(connection, login);
		if (!utils::getInt(connection)) break;
		std::cout << "��� ���������� �����. ������� ������ �����.\n";
	}

	std::cout << "������� ������: ";
	password = utils::inputString();
	utils::sendString(connection, password);

	std::cout << "\n����������� ������ �������.\n";
	utils::pause();
}

void addDocuments(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "�� �������� � �������, ������� �� ������ ������� ���������.\n";
		return;
	}
	auto documents = std::vector <int>();
	int transportType = utils::getInt(connection);
	while (true) {
		utils::clearScreen();
		if (!utils::getInt(connection)) outputDocuments(connection, documents);
		std::cout << "\n��������, ����� ��������� ������: \n\n";
		int answer = 0, count = 0;
		count++;
		std::cout << count << ". �������;\n";
		if (transportType > 1) {
			count++;
			std::cout << count << ". ����������;\n";
			count++;
			std::cout << count << ". ������������ �������������;\n";
			count++;
			std::cout << count << ". CMR �����������;\n";
		}
		if (transportType == 4) {
			count++;
			std::cout << count << ". ����-�������;\n";
			count++;
			std::cout << count << ". ����������� ����;\n";
			count++;
			std::cout << count << ". ���������� ������������� ������;\n";
			count++;
			std::cout << count << ". ���������� ����������;\n";
		}
		std::cout << "\n��� �����: ";
		utils::enterInInterval(answer, 1, count);
		for (auto document : documents)
			if (answer == document) answer = 0;
		if (answer == 0) {
			utils::sendInt(connection, 0);
			std::cout << "���� �������� ��� ��������.\n";
		} else {
			utils::sendInt(connection, 1);
			utils::sendInt(connection, answer);
			documents.push_back(answer);
		}
		if (!utils::YesOrNo("���������� ���� ����������? y/n")) {
			utils::sendInt(connection, 0);
			break;
		}
		utils::sendInt(connection, 1);
	}
}

bool removeUser(SOCKET& connection) {
	if (utils::YesOrNo("�� �������? y/n")) {
		utils::sendInt(connection, 1);
		return true;
	}
	utils::sendInt(connection, 0);
	return false;
}

void registeringUserInQueue(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "��� �������� � �������.\n";
		return;
	}
	outputUsers(connection);
	selectingId(connection);
	if (utils::getInt(connection)) return;

	utils::clearScreen();
	outputUsers(connection);

	if (utils::YesOrNo("������� � �������? y/n ")) {
		utils::sendInt(connection, 1);
		std::cout << "\n������������ �������� � �������.\n";
	} else {
		utils::sendInt(connection, 0);
		std::cout << "\n������ ������������ ��������.\n";
	}
}

void printingQueue(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "������� �����.\n";
		return;
	}
	std::ofstream fout("Queue.txt", std::ios_base::trunc);
	if (!fout.is_open()) {
		utils::sendInt(connection, 0);
		std::cout << "���� �� ����� ���� ������.\n";
		return;
	}
	utils::sendInt(connection, 1);

	fout.fill('-');
	fout << " +" << std::setw(11) << "+" << std::setw(27) << "+" << std::setw(14) << "+" << std::setw(18) << "+" << std::setw(34) << "+\n";
	fout.fill(' ');
	fout << " |" << std::setw(6) << "ID" << std::setw(5) << "|" << std::setw(14) << "��" << std::setw(13) << "|"
		<< std::setw(10) << "������" << std::setw(4) << "|" << std::setw(12) << "�������" << std::setw(6) << "|"
		<< std::setw(21) << "���������" << std::setw(13) << "|\n";
	fout << " |" << std::setw(11) << "|" << std::setw(27) << "|" << std::setw(12) << "�����������" << std::setw(2) << "|"
		<< std::setw(13) << "���������" << std::setw(5) << "|" << std::setw(34) << "|\n";
	fout.fill('-');
	fout << " +" << std::setw(11) << "+" << std::setw(27) << "+" << std::setw(14) << "+" << std::setw(18) << "+" << std::setw(34) << "+\n";
	fout.fill(' ');

	int ID = 0, answer = 0, count = 0;
	std::string SN, document;
	int size = utils::getInt(connection);
	for (int i = 0; i < size; i++) {
		ID = utils::getInt(connection);
		SN = utils::getString(connection);
		fout << " |" << std::setw(8) << ID << std::setw(3) << "|" <<
			std::setw(23) << SN << std::setw(4) << "|";
		answer = utils::getInt(connection);
		switch (answer) {
		case 1: fout << std::setw(10) << "������" << std::setw(4) << "|";
			break;
		case 2: fout << std::setw(10) << "������" << std::setw(4) << "|";
			break;
		case 3: fout << std::setw(10) << "�������" << std::setw(4) << "|";
			break;
		case 4: fout << std::setw(9) << "����" << std::setw(5) << "|";
			break;
		}
		answer = utils::getInt(connection);
		switch (answer) {
		case 1: fout << std::setw(16) << "���������������" << std::setw(2) << "|";
			break;
		case 2: fout << std::setw(14) << "����������" << std::setw(4) << "|";
			break;
		case 3: fout << std::setw(14) << "����������" << std::setw(4) << "|";
			break;
		case 4: fout << std::setw(11) << "����" << std::setw(7) << "|";
			break;
		}
		count = utils::getInt(connection);
		if (count) {
			int docType = 0;
			for (int j = 0; j < count; j++) {
				docType = utils::getInt(connection);
				switch (docType) {
				case 1: document = "�������"; break;
				case 2: document = "����������"; break;
				case 3: document = "������������ �������������"; break;
				case 4: document = "CMR �����������"; break;
				case 5: document = "����-�������"; break;
				case 6: document = "����������� ����"; break;
				case 7: document = "���������� ������������� ������"; break;
				case 8: document = "���������� ����������"; break;
				}

				if (j == 0) fout << std::setw(32) << document << std::setw(2) << "|\n";
				else if (j == 1 && answer == 1)
					fout << " |" << std::setw(11) << "|" << std::setw(27) << "|" << std::setw(14) << "|"
					<< std::setw(14) << "����������" << std::setw(4) << "|" << std::setw(32) << document << std::setw(2) << "|\n";
				else fout << " |" << std::setw(11) << "|" << std::setw(27) << "|" << std::setw(14) << "|"
					<< std::setw(18) << "|" << std::setw(32) << document << std::setw(2) << "|\n";
			}
		} else fout << std::setw(17) << "-" << std::setw(17) << "|\n";

		fout.fill('-');
		fout << " +" << std::setw(11) << "+" << std::setw(27) << "+" << std::setw(14) << "+" << std::setw(18) << "+" << std::setw(34) << "+\n";
		fout.fill(' ');

		if (!fout) {
			utils::sendInt(connection, 0);
			std::cout << "������ � ���� ����������.\n";
			return;
		}
		utils::sendInt(connection, 1);
	}
	fout.close();

	std::cout << "������ ���������.\n";
}

void usersPassThroughCustoms(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "������� �����.\n";
		return;
	}
	outputUsers(connection);
	utils::YesOrNo("����������� ����� �������? y/n");
}

void personalAccount(SOCKET& connection) {
	std::string valString;
	int valInt = 0;
	valString = utils::getString(connection);
	std::cout << "��� � �������: " << valString << "\n";
	tm dayOfBirth;
	dayOfBirth.tm_mday = utils::getInt(connection);
	dayOfBirth.tm_mon = utils::getInt(connection);
	dayOfBirth.tm_year = utils::getInt(connection);
	std::cout << "���� ��������: ";
	utils::outputDate(dayOfBirth);
	std::cout << "\n";
	valInt = utils::getInt(connection);
	std::cout << "����������������� �����: " << valInt << "\n";
	std::cout << "������ ����������� �������: ";
	valInt = utils::getInt(connection);
	switch (valInt) {
	case 1: std::cout << "������\n";
		break;
	case 2:std::cout << "������\n";
		break;
	case 3:std::cout << "�������\n";
		break;
	case 4:std::cout << "����\n";
		break;
	}
	if (valInt == 4) {
		std::cout << "����������� ����: ";
		valInt = utils::getInt(connection);
		switch (valInt) {
		case 1: std::cout << "��������������� ����������\n";
			break;
		case 2: std::cout << "����������\n";
			break;
		case 3: std::cout << "����������\n";
			break;
		}
	}
	valInt = utils::getInt(connection);
	std::cout << "���������:\n";
	if (valInt) {
		int count = valInt;
		for (int j = 1; j <= count; j++) {
			std::cout << std::setw(11) << j << ". ";
			valInt = utils::getInt(connection);
			switch (valInt) {
			case 1: std::cout << "�������\n"; break;
			case 2: std::cout << "����������\n"; break;
			case 3: std::cout << "������������ �������������\n"; break;
			case 4: std::cout << "CMR �����������\n"; break;
			case 5: std::cout << "����-�������\n"; break;
			case 6: std::cout << "����������� ����\n"; break;
			case 7: std::cout << "���������� ������������� ������\n"; break;
			case 8: std::cout << "���������� ����������\n"; break;
			}
		}
	} else std::cout << "-\n";
}

void sendingRequest(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "� ��� �� ��������� ���������.\n";
		return;
	}
	int type = utils::getInt(connection);
	switch (type) {
	case 2:
		std::cout << "�� ��� ��������� ������.\n";
		return;
	case 4:
		std::cout << "�� ��� �������� � �������.\n";
		return;
	}

	if (utils::getInt(connection))
		std::cout << "������ ���������.\n";
}

void requestVerification(SOCKET& connection) {
	int number = 0;
	int type = utils::getInt(connection);
	switch (type) {
	case 1:
		std::cout << "�� �� ���������� ������.\n";
		break;
	case 2:
		std::cout << "��� ������ ��� �� ����������.\n";
		break;
	case 3:
		std::cout << "������ ��������.\n";
		break;
	case 4:
		number = utils::getInt(connection);
		std::cout << "��� ����� � �������: " << number << ".\n";
		break;
	}
}

void editExpertsScores(SOCKET& connection) {
	int menu = 0, size = 0, score = 0;
	std::cout << "�������� ��������:\n"
		<< "1. ������� �� ������� ��������;\n"
		<< "2. ������� �� ������� ����������� � �������� ���������;\n"
		<< "3. ��������� �����.\n"
		<< "��� �����: ";
	utils::enterInInterval(menu, 1, 3);
	utils::sendInt(connection, menu);
	utils::clearScreen();
	switch (menu) {
	case 1:
		outputFirstExpertsScores(connection);
		size = 11;
		break;
	case 2:
		outputSecondExpertsScores(connection);
		size = 6;
		break;
	case 3: return;
	}
	std::cout << "������� ����� ��������: \n";
	for (int i = 0; i < size; i++) {
		utils::enterInInterval(score, 0, 10);
		utils::sendInt(connection, score);
	}
	std::cout << "\n������ �������� ��������.\n";
}

void editData(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "�� �������� � �������, ������� �� ������ �������� ������.\n";
		return;
	}
	int menu = 0, type = 0;
	std::cout << "�������� ������ ��� ���������:\n"
		<< "1. ������ ����������� �������;\n"
		<< "2. ������� ���������;\n"
		<< "3. ��������� �����.\n"
		<< "��� �����: ";
	utils::enterInInterval(menu, 1, 3);
	utils::sendInt(connection, menu);
	utils::clearScreen();
	switch (menu) {
	case 1:
	{
		std::cout << "������� ������ ����������� �������: ";
		type = utils::getInt(connection);
		switch (type) {
		case 1: std::cout << "������\n";
			break;
		case 2:std::cout << "������\n";
			break;
		case 3:std::cout << "�������\n";
			break;
		case 4:std::cout << "����\n";
			break;
		}
		std::cout << "�������� �����: \n"
			<< "1. ������;\n"
			<< "2. ������;\n"
			<< "3. �������;\n"
			<< "4. ����;\n";
		utils::enterInInterval(type, 1, 4);
		utils::sendInt(connection, type);
		if (type == 4) {
			std::cout << "�������� ������� ���������:\n"
				<< "1. ��������������� ����������;\n"
				<< "2. ����������;\n"
				<< "3. ����������;\n";
			utils::enterInInterval(type, 1, 3);
			utils::sendInt(connection, type);
		}
	}
	break;
	case 2:
	{
		if (utils::getInt(connection) != 4) {
			std::cout << "���������� �������� ������� ���������, ���� �������� ����������� �� �������� ����.\n";
			return;
		}
		std::cout << "������� ������� ���������: ";
		type = utils::getInt(connection);
		switch (type) {
		case 1: std::cout << "��������������� ����������\n";
			break;
		case 2: std::cout << "����������\n";
			break;
		case 3: std::cout << "����������\n";
			break;
		}
		std::cout << "�������� �����:\n"
			<< "1. ��������������� ����������;\n"
			<< "2. ����������;\n"
			<< "3. ����������;\n";
		utils::enterInInterval(type, 1, 3);
		utils::sendInt(connection, type);
	}
	break;
	case 3: return;
	}

	std::cout << "\n������ ��������.\n";
}

void outputUsers(SOCKET& connection) {
	std::cout.fill('-');
	std::cout << " +" << std::setw(11) << "+" << std::setw(27) << "+" << std::setw(14) << "+" << std::setw(18) << "+" << std::setw(34) << "+\n";
	std::cout.fill(' ');
	std::cout << " |" << std::setw(6) << "ID" << std::setw(5) << "|" << std::setw(14) << "��" << std::setw(13) << "|"
		<< std::setw(10) << "������" << std::setw(4) << "|" << std::setw(12) << "�������" << std::setw(6) << "|"
		<< std::setw(21) << "���������" << std::setw(13) << "|\n";
	std::cout << " |" << std::setw(11) << "|" << std::setw(27) << "|" << std::setw(12) << "�����������" << std::setw(2) << "|"
		<< std::setw(13) << "���������" << std::setw(5) << "|" << std::setw(34) << "|\n";
	std::cout.fill('-');
	std::cout << " +" << std::setw(11) << "+" << std::setw(27) << "+" << std::setw(14) << "+" << std::setw(18) << "+" << std::setw(34) << "+\n";
	std::cout.fill(' ');

	int ID = 0, answer = 0, count = 0;
	std::string SN, document;
	int size = utils::getInt(connection);
	for (int i = 1; i <= size; i++) {
		ID = utils::getInt(connection);
		SN = utils::getString(connection);
		std::cout << " |" << std::setw(8) << ID << std::setw(3) << "|" <<
			std::setw(23) << SN << std::setw(4) << "|";
		answer = utils::getInt(connection);
		switch (answer) {
		case 1: std::cout << std::setw(10) << "������" << std::setw(4) << "|";
			break;
		case 2:std::cout << std::setw(10) << "������" << std::setw(4) << "|";
			break;
		case 3:std::cout << std::setw(10) << "�������" << std::setw(4) << "|";
			break;
		case 4:std::cout << std::setw(9) << "����" << std::setw(5) << "|";
			break;
		}
		answer = utils::getInt(connection);
		switch (answer) {
		case 1: std::cout << std::setw(16) << "���������������" << std::setw(2) << "|";
			break;
		case 2: std::cout << std::setw(14) << "����������" << std::setw(4) << "|";
			break;
		case 3: std::cout << std::setw(14) << "����������" << std::setw(4) << "|";
			break;
		case 4: std::cout << std::setw(11) << "����" << std::setw(7) << "|";
			break;
		}
		count = utils::getInt(connection);
		if (count) {
			int docType = 0;
			for (int j = 0; j < count; j++) {
				docType = utils::getInt(connection);
				switch (docType) {
				case 1: document = "�������"; break;
				case 2: document = "����������"; break;
				case 3: document = "������������ �������������"; break;
				case 4: document = "CMR �����������"; break;
				case 5: document = "����-�������"; break;
				case 6: document = "����������� ����"; break;
				case 7: document = "���������� ������������� ������"; break;
				case 8: document = "���������� ����������"; break;
				}

				if (j == 0) std::cout << std::setw(32) << document << std::setw(2) << "|\n";
				else if (j == 1 && answer == 1)
					std::cout << " |" << std::setw(11) << "|" << std::setw(27) << "|" << std::setw(14) << "|"
					<< std::setw(14) << "����������" << std::setw(4) << "|" << std::setw(32) << document << std::setw(2) << "|\n";
				else std::cout << " |" << std::setw(11) << "|" << std::setw(27) << "|" << std::setw(14) << "|"
					<< std::setw(18) << "|" << std::setw(32) << document << std::setw(2) << "|\n";
			}
		} else std::cout << std::setw(17) << "-" << std::setw(17) << "|\n";
		std::cout.fill('-');
		std::cout << " +" << std::setw(11) << "+" << std::setw(27) << "+" << std::setw(14) << "+" << std::setw(18) << "+" << std::setw(34) << "+\n";
		std::cout.fill(' ');
	}
	std::cout << "\n";
}

void outputFirstExpertsScores(SOCKET& connection) {
	std::cout << "������� 1: �� ������� ��������\n\n";
	std::cout.fill('-');
	std::cout << " +" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+"
		<< std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(13) << "+\n";
	std::cout.fill(' ');
	std::cout << " |" << std::setw(8) << "������" << std::setw(4) << "|" << std::setw(8) << "������" << std::setw(4) << "|"
		<< std::setw(8) << "������" << std::setw(4) << "|" << std::setw(10) << "���������" << std::setw(2) << "|"
		<< std::setw(8) << "�����" << std::setw(4) << "|" << std::setw(8) << "������" << std::setw(4) << "|"
		<< std::setw(9) << "�������" << std::setw(3) << "|" << std::setw(9) << "�������" << std::setw(3) << "|"
		<< std::setw(9) << "�������" << std::setw(3) << "|" << std::setw(9) << "�������" << std::setw(3) << "|"
		<< std::setw(10) << "���������" << std::setw(3) << "|\n";
	std::cout.fill('-');
	std::cout << " +" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+"
		<< std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(13) << "+\n";
	std::cout.fill(' ');

	int score = 0;
	for (int i = 0; i < 11; i++) {
		score = utils::getInt(connection);
		if (i == 0) std::cout << " |" << std::setw(6) << score << std::setw(6) << "|";
		else if (i == 10) std::cout << std::setw(6) << score << std::setw(7) << "|\n";
		else std::cout << std::setw(6) << score << std::setw(6) << "|";
	}
	std::cout.fill('-');
	std::cout << " +" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+"
		<< std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(13) << "+\n";
	std::cout.fill(' ');
	std::cout << "\n";
}

void outputSecondExpertsScores(SOCKET& connection) {
	std::cout << "������� 2: �� ������� ����������� � �������� ���������\n\n";
	std::cout.fill('-');
	std::cout << " +" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+\n";
	std::cout.fill(' ');
	std::cout << " |" << std::setw(12) << "������" << std::setw(7) << "|" << std::setw(12) << "������" << std::setw(7) << "|"
		<< std::setw(12) << "�������" << std::setw(7) << "|" << std::setw(12) << "���� ��" << std::setw(7) << "|"
		<< std::setw(12) << "���� �" << std::setw(7) << "|" << std::setw(12) << "���� �" << std::setw(7) << "|\n";
	std::cout << " |" << std::setw(19) << "|" << std::setw(19) << "|" << std::setw(19) << "|" << std::setw(17) << "����������������" << std::setw(2) << "|"
		<< std::setw(15) << "������������" << std::setw(4) << "|" << std::setw(15) << "������������" << std::setw(4) << "|\n";
	std::cout << " |" << std::setw(19) << "|" << std::setw(19) << "|" << std::setw(19) << "|"
		<< std::setw(15) << "������������" << std::setw(4) << "|" << std::setw(19) << "|" << std::setw(19) << "|\n";
	std::cout.fill('-');
	std::cout << " +" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+\n";
	std::cout.fill(' ');

	int score = 0;
	for (int i = 0; i < 6; i++) {
		score = utils::getInt(connection);
		if (i == 0) std::cout << " |" << std::setw(10) << score << std::setw(9) << "|";
		else if (i == 5) std::cout << std::setw(10) << score << std::setw(9) << "|\n";
		else std::cout << std::setw(10) << score << std::setw(9) << "|";
	}
	std::cout.fill('-');
	std::cout << " +" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+\n";
	std::cout.fill(' ');
	std::cout << "\n";
}

void outputDocuments(SOCKET& connection, std::vector <int> documents) {
	int valInt = utils::getInt(connection);
	std::cout << "���������: \n";
	if (valInt) {
		int count = valInt;
		for (int j = 1; j <= count; j++) {
			std::cout << std::setw(11) << j << ". ";
			valInt = utils::getInt(connection);
			documents.push_back(valInt);
			switch (valInt) {
			case 1: std::cout << "�������\n"; break;
			case 2: std::cout << "����������\n"; break;
			case 3: std::cout << "������������ �������������\n"; break;
			case 4: std::cout << "CMR �����������\n"; break;
			case 5: std::cout << "����-�������\n"; break;
			case 6: std::cout << "����������� ����\n"; break;
			case 7: std::cout << "���������� ������������� ������\n"; break;
			case 8: std::cout << "���������� ����������\n"; break;
			}
		}
	}
}

void selectingId(SOCKET& connection) {
	while (true) {
		std::cout << "������� ID ������� ������������: ";
		int id = 0;
		utils::inputCount(id);
		utils::sendInt(connection, id);
		if (utils::getInt(connection))
			return;
		else
			std::cout << "������ �������� ID. ";
		if (!utils::YesOrNo("������� ���������? y/n ")) {
			utils::sendInt(connection, 0);
			return;
		}
		utils::sendInt(connection, 1);
		std::cout << "\n";
	}
}
