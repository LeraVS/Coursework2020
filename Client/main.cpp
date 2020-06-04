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
		std::cout << "Главное меню\n\n"
			<< "1. Вход.\n"
			<< "2. Регистрация.\n"
			<< "3. Выход.\n"
			<< "Ваш выбор: ";
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
		std::cout << "Введите логин: ";
		login = utils::inputString();
		utils::sendString(connection, login);
		std::cout << "Введите пароль: ";
		password = utils::inputString();
		utils::sendString(connection, password);
		return true;
	}
	while (i) {
		std::cout << "Введите логин: ";
		login = utils::inputString();
		utils::sendString(connection, login);
		std::cout << "Введите пароль: ";
		password = utils::inputPassword();
		utils::sendString(connection, password);
		if (utils::getInt(connection))
			return true;
		std::cout << "Неверный ввод. Осталось " << i - 1 << " попыток.\n";
		i--;
		if (i == 0) break;
		if (!utils::YesOrNo("Желаете повторить? y/n ")) {
			utils::sendInt(connection, 0);
			return false;
		}
		utils::sendInt(connection, 1);
		std::cout << "\n";
	}
	if (i == 0)
		std::cout << "Исчерпано количество попыток.\n";
	utils::pause();
	return false;
}

bool authorizationUser(SOCKET& connection) {
	utils::clearScreen();
	std::string login, password;
	int i = 3;
	while (i) {
		std::cout << "Введите логин: ";
		login = utils::inputString();
		utils::sendString(connection, login);
		std::cout << "Введите пароль: ";
		password = utils::inputPassword();
		utils::sendString(connection, password);
		if (utils::getInt(connection))
			return true;
		std::cout << "Неверный ввод. Осталось " << i - 1 << " попыток.\n";
		i--;
		if (i == 0) break;
		if (!utils::YesOrNo("Желаете повторить? y/n ")) {
			utils::sendInt(connection, 0);
			return false;
		}
		utils::sendInt(connection, 1);
		std::cout << "\n";
	}
	if (i == 0)
		std::cout << "Исчерпано количество попыток.\n";
	utils::pause();
	return false;
}

void enterMenu(SOCKET& connection) {
	int menu = 0;
	while (true) {
		utils::clearScreen();
		std::cout << "Выберите аккаунт для входа:\n\n"
			<< "1. Администратор.\n"
			<< "2. Пользователь.\n"
			<< "3. Вернуться назад.\n"
			<< "Ваш выбор: \n";
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
		std::cout << "Меню администратора:\n\n"
			<< "1. Просмотр заявок в очередь;\n"
			<< "2. Просмотр очереди;\n"
			<< "3. Печать очереди;\n"
			<< "4. Пропуск пользователя через таможню;\n"
			<< "5. Изменение экспертных оценок;\n"
			<< "6. Вернуться назад.\n"
			<< "Ваш выбор: ";
		utils::enterInInterval(menu, 1, 6);
		utils::sendInt(connection, menu);
		utils::clearScreen();
		switch (menu) {
		case 1: registeringUserInQueue(connection); break;
		case 2: if (utils::getInt(connection)) std::cout << "Очередь пуста.\n";
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
		std::cout << "Меню пользователя:\n\n"
			<< "1. Личный кабинет;\n"
			<< "2. Внесение документов;\n"
			<< "3. Изменение данных о пересечении границы;\n"
			<< "4. Запрос в очередь;\n"
			<< "5. Просмотр номера в очереди;\n"
			<< "6. Удаление аккаунта;\n"
			<< "7. Вернуться назад.\n"
			<< "Ваш выбор: ";
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
	std::cout << "Выберите действие:\n\n"
		<< "1. Отправка запроса;\n"
		<< "2. Проверка запроса;\n"
		<< "3. Вернуться назад.\n"
		<< "Ваш выбор: ";
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
	std::cout << "Фамилия: ";
	surname = utils::inputString();
	utils::sendString(connection, surname);
	std::cout << "Имя: ";
	name = utils::inputString();
	utils::sendString(connection, name);
	std::cout << "Идентификационный номер: ";
	utils::inputCount(ID);
	utils::sendInt(connection, ID);
	std::cout << "День рождения: \n";
	dayOfBirth = utils::inputDate();
	utils::sendInt(connection, dayOfBirth.tm_mday);
	utils::sendInt(connection, dayOfBirth.tm_mon);
	utils::sendInt(connection, dayOfBirth.tm_year);
	std::cout << "\n";
	int answer = 0;
	std::cout << "Способ пересечения границы:\n"
		<< "1. Пешком;\n"
		<< "2. Машина;\n"
		<< "3. Автобус;\n"
		<< "4. Фура;\n";
	utils::enterInInterval(answer, 1, 4);
	utils::sendInt(connection, answer);

	if (answer == 4) {
		std::cout << "\nПеревозимый груз:\n"
			<< "1. Скоропортящиеся продтовары;\n"
			<< "2. Продтовары;\n"
			<< "3. Промтовары;\n";
		utils::enterInInterval(answer, 1, 3);
		utils::sendInt(connection, answer);
	}
	std::cout << "\n";
	std::cout << "Введите логин: ";
	while (true) {
		login = utils::inputString();
		utils::sendString(connection, login);
		if (!utils::getInt(connection)) break;
		std::cout << "Уже существует логин. Введите другой логин.\n";
	}

	std::cout << "Введите пароль: ";
	password = utils::inputString();
	utils::sendString(connection, password);

	std::cout << "\nРегистрация прошла успешно.\n";
	utils::pause();
}

void addDocuments(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "Вы состоите в очереди, поэтому не можете вносить документы.\n";
		return;
	}
	auto documents = std::vector <int>();
	int transportType = utils::getInt(connection);
	while (true) {
		utils::clearScreen();
		if (!utils::getInt(connection)) outputDocuments(connection, documents);
		std::cout << "\nВыберите, какие документы внести: \n\n";
		int answer = 0, count = 0;
		count++;
		std::cout << count << ". Паспорт;\n";
		if (transportType > 1) {
			count++;
			std::cout << count << ". Техпаспорт;\n";
			count++;
			std::cout << count << ". Водительское удостоверение;\n";
			count++;
			std::cout << count << ". CMR страхование;\n";
		}
		if (transportType == 4) {
			count++;
			std::cout << count << ". Счёт-фактура;\n";
			count++;
			std::cout << count << ". Упаковочный лист;\n";
			count++;
			std::cout << count << ". Сертификат происхождения товара;\n";
			count++;
			std::cout << count << ". Транзитная декларация;\n";
		}
		std::cout << "\nВаш выбор: ";
		utils::enterInInterval(answer, 1, count);
		for (auto document : documents)
			if (answer == document) answer = 0;
		if (answer == 0) {
			utils::sendInt(connection, 0);
			std::cout << "Этот документ уже добавили.\n";
		} else {
			utils::sendInt(connection, 1);
			utils::sendInt(connection, answer);
			documents.push_back(answer);
		}
		if (!utils::YesOrNo("Продолжить ввод документов? y/n")) {
			utils::sendInt(connection, 0);
			break;
		}
		utils::sendInt(connection, 1);
	}
}

bool removeUser(SOCKET& connection) {
	if (utils::YesOrNo("Вы уверены? y/n")) {
		utils::sendInt(connection, 1);
		return true;
	}
	utils::sendInt(connection, 0);
	return false;
}

void registeringUserInQueue(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "Нет запросов в очередь.\n";
		return;
	}
	outputUsers(connection);
	selectingId(connection);
	if (utils::getInt(connection)) return;

	utils::clearScreen();
	outputUsers(connection);

	if (utils::YesOrNo("Принять в очередь? y/n ")) {
		utils::sendInt(connection, 1);
		std::cout << "\nПользователь добавлен в очередь.\n";
	} else {
		utils::sendInt(connection, 0);
		std::cout << "\nЗапрос пользователя отклонен.\n";
	}
}

void printingQueue(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "Очередь пуста.\n";
		return;
	}
	std::ofstream fout("Queue.txt", std::ios_base::trunc);
	if (!fout.is_open()) {
		utils::sendInt(connection, 0);
		std::cout << "Файл не может быть открыт.\n";
		return;
	}
	utils::sendInt(connection, 1);

	fout.fill('-');
	fout << " +" << std::setw(11) << "+" << std::setw(27) << "+" << std::setw(14) << "+" << std::setw(18) << "+" << std::setw(34) << "+\n";
	fout.fill(' ');
	fout << " |" << std::setw(6) << "ID" << std::setw(5) << "|" << std::setw(14) << "ФИ" << std::setw(13) << "|"
		<< std::setw(10) << "Способ" << std::setw(4) << "|" << std::setw(12) << "Объекты" << std::setw(6) << "|"
		<< std::setw(21) << "Документы" << std::setw(13) << "|\n";
	fout << " |" << std::setw(11) << "|" << std::setw(27) << "|" << std::setw(12) << "пересечения" << std::setw(2) << "|"
		<< std::setw(13) << "перевозки" << std::setw(5) << "|" << std::setw(34) << "|\n";
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
		case 1: fout << std::setw(10) << "Пешком" << std::setw(4) << "|";
			break;
		case 2: fout << std::setw(10) << "Машина" << std::setw(4) << "|";
			break;
		case 3: fout << std::setw(10) << "Автобус" << std::setw(4) << "|";
			break;
		case 4: fout << std::setw(9) << "Фура" << std::setw(5) << "|";
			break;
		}
		answer = utils::getInt(connection);
		switch (answer) {
		case 1: fout << std::setw(16) << "Скоропортящиеся" << std::setw(2) << "|";
			break;
		case 2: fout << std::setw(14) << "Продтовары" << std::setw(4) << "|";
			break;
		case 3: fout << std::setw(14) << "Промтовары" << std::setw(4) << "|";
			break;
		case 4: fout << std::setw(11) << "Люди" << std::setw(7) << "|";
			break;
		}
		count = utils::getInt(connection);
		if (count) {
			int docType = 0;
			for (int j = 0; j < count; j++) {
				docType = utils::getInt(connection);
				switch (docType) {
				case 1: document = "Паспорт"; break;
				case 2: document = "Техпаспорт"; break;
				case 3: document = "Водительское удостоверение"; break;
				case 4: document = "CMR страхование"; break;
				case 5: document = "Счёт-фактура"; break;
				case 6: document = "Упаковочный лист"; break;
				case 7: document = "Сертификат происхождения товара"; break;
				case 8: document = "Транзитная декларация"; break;
				}

				if (j == 0) fout << std::setw(32) << document << std::setw(2) << "|\n";
				else if (j == 1 && answer == 1)
					fout << " |" << std::setw(11) << "|" << std::setw(27) << "|" << std::setw(14) << "|"
					<< std::setw(14) << "продтовары" << std::setw(4) << "|" << std::setw(32) << document << std::setw(2) << "|\n";
				else fout << " |" << std::setw(11) << "|" << std::setw(27) << "|" << std::setw(14) << "|"
					<< std::setw(18) << "|" << std::setw(32) << document << std::setw(2) << "|\n";
			}
		} else fout << std::setw(17) << "-" << std::setw(17) << "|\n";

		fout.fill('-');
		fout << " +" << std::setw(11) << "+" << std::setw(27) << "+" << std::setw(14) << "+" << std::setw(18) << "+" << std::setw(34) << "+\n";
		fout.fill(' ');

		if (!fout) {
			utils::sendInt(connection, 0);
			std::cout << "Запись в файл невозможна.\n";
			return;
		}
		utils::sendInt(connection, 1);
	}
	fout.close();

	std::cout << "Печать завершена.\n";
}

void usersPassThroughCustoms(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "Очередь пуста.\n";
		return;
	}
	outputUsers(connection);
	utils::YesOrNo("Пропускаете через границу? y/n");
}

void personalAccount(SOCKET& connection) {
	std::string valString;
	int valInt = 0;
	valString = utils::getString(connection);
	std::cout << "Имя и фамилия: " << valString << "\n";
	tm dayOfBirth;
	dayOfBirth.tm_mday = utils::getInt(connection);
	dayOfBirth.tm_mon = utils::getInt(connection);
	dayOfBirth.tm_year = utils::getInt(connection);
	std::cout << "День рождения: ";
	utils::outputDate(dayOfBirth);
	std::cout << "\n";
	valInt = utils::getInt(connection);
	std::cout << "Идентификационный номер: " << valInt << "\n";
	std::cout << "Способ пересечения границы: ";
	valInt = utils::getInt(connection);
	switch (valInt) {
	case 1: std::cout << "пешком\n";
		break;
	case 2:std::cout << "машина\n";
		break;
	case 3:std::cout << "автобус\n";
		break;
	case 4:std::cout << "фура\n";
		break;
	}
	if (valInt == 4) {
		std::cout << "Перевозимый груз: ";
		valInt = utils::getInt(connection);
		switch (valInt) {
		case 1: std::cout << "скоропортящиеся продтовары\n";
			break;
		case 2: std::cout << "продтовары\n";
			break;
		case 3: std::cout << "промтовары\n";
			break;
		}
	}
	valInt = utils::getInt(connection);
	std::cout << "Документы:\n";
	if (valInt) {
		int count = valInt;
		for (int j = 1; j <= count; j++) {
			std::cout << std::setw(11) << j << ". ";
			valInt = utils::getInt(connection);
			switch (valInt) {
			case 1: std::cout << "Паспорт\n"; break;
			case 2: std::cout << "Техпаспорт\n"; break;
			case 3: std::cout << "Водительское удостоверение\n"; break;
			case 4: std::cout << "CMR страхование\n"; break;
			case 5: std::cout << "Счёт-фактура\n"; break;
			case 6: std::cout << "Упаковочный лист\n"; break;
			case 7: std::cout << "Сертификат происхождения товара\n"; break;
			case 8: std::cout << "Транзитная декларация\n"; break;
			}
		}
	} else std::cout << "-\n";
}

void sendingRequest(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "У вас не добавлены документы.\n";
		return;
	}
	int type = utils::getInt(connection);
	switch (type) {
	case 2:
		std::cout << "Вы уже отправили запрос.\n";
		return;
	case 4:
		std::cout << "Вы уже состоите в очереди.\n";
		return;
	}

	if (utils::getInt(connection))
		std::cout << "Запрос отправлен.\n";
}

void requestVerification(SOCKET& connection) {
	int number = 0;
	int type = utils::getInt(connection);
	switch (type) {
	case 1:
		std::cout << "Вы не отправляли запрос.\n";
		break;
	case 2:
		std::cout << "Ваш запрос еще не рассмотрен.\n";
		break;
	case 3:
		std::cout << "Запрос отклонен.\n";
		break;
	case 4:
		number = utils::getInt(connection);
		std::cout << "Ваш номер в очереди: " << number << ".\n";
		break;
	}
}

void editExpertsScores(SOCKET& connection) {
	int menu = 0, size = 0, score = 0;
	std::cout << "Выберите эксперта:\n"
		<< "1. Эксперт по времени прибытия;\n"
		<< "2. Эксперт по способу пересечения и объектам перевозки;\n"
		<< "3. Вернуться назад.\n"
		<< "Ваш выбор: ";
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
	std::cout << "Введите новые значения: \n";
	for (int i = 0; i < size; i++) {
		utils::enterInInterval(score, 0, 10);
		utils::sendInt(connection, score);
	}
	std::cout << "\nОценки эксперта изменены.\n";
}

void editData(SOCKET& connection) {
	if (utils::getInt(connection)) {
		std::cout << "Вы состоите в очереди, поэтому не можете изменять данные.\n";
		return;
	}
	int menu = 0, type = 0;
	std::cout << "Выберите данные для изменения:\n"
		<< "1. Способ пересечения границы;\n"
		<< "2. Объекты перевозки;\n"
		<< "3. Вернуться назад.\n"
		<< "Ваш выбор: ";
	utils::enterInInterval(menu, 1, 3);
	utils::sendInt(connection, menu);
	utils::clearScreen();
	switch (menu) {
	case 1:
	{
		std::cout << "Текущий способ пересечения границы: ";
		type = utils::getInt(connection);
		switch (type) {
		case 1: std::cout << "пешком\n";
			break;
		case 2:std::cout << "машина\n";
			break;
		case 3:std::cout << "автобус\n";
			break;
		case 4:std::cout << "фура\n";
			break;
		}
		std::cout << "Выберите новый: \n"
			<< "1. Пешком;\n"
			<< "2. Машина;\n"
			<< "3. Автобус;\n"
			<< "4. Фура;\n";
		utils::enterInInterval(type, 1, 4);
		utils::sendInt(connection, type);
		if (type == 4) {
			std::cout << "Выберите объекты перевозки:\n"
				<< "1. Скоропортящиеся продтовары;\n"
				<< "2. Продтовары;\n"
				<< "3. Промтовары;\n";
			utils::enterInInterval(type, 1, 3);
			utils::sendInt(connection, type);
		}
	}
	break;
	case 2:
	{
		if (utils::getInt(connection) != 4) {
			std::cout << "Невозможно изменить объекты перевозки, пока способом пересечения не является фура.\n";
			return;
		}
		std::cout << "Текущие объекты перевозки: ";
		type = utils::getInt(connection);
		switch (type) {
		case 1: std::cout << "скоропортящиеся продтовары\n";
			break;
		case 2: std::cout << "продтовары\n";
			break;
		case 3: std::cout << "промтовары\n";
			break;
		}
		std::cout << "Выберите новые:\n"
			<< "1. Скоропортящиеся продтовары;\n"
			<< "2. Продтовары;\n"
			<< "3. Промтовары;\n";
		utils::enterInInterval(type, 1, 3);
		utils::sendInt(connection, type);
	}
	break;
	case 3: return;
	}

	std::cout << "\nДанные изменены.\n";
}

void outputUsers(SOCKET& connection) {
	std::cout.fill('-');
	std::cout << " +" << std::setw(11) << "+" << std::setw(27) << "+" << std::setw(14) << "+" << std::setw(18) << "+" << std::setw(34) << "+\n";
	std::cout.fill(' ');
	std::cout << " |" << std::setw(6) << "ID" << std::setw(5) << "|" << std::setw(14) << "ФИ" << std::setw(13) << "|"
		<< std::setw(10) << "Способ" << std::setw(4) << "|" << std::setw(12) << "Объекты" << std::setw(6) << "|"
		<< std::setw(21) << "Документы" << std::setw(13) << "|\n";
	std::cout << " |" << std::setw(11) << "|" << std::setw(27) << "|" << std::setw(12) << "пересечения" << std::setw(2) << "|"
		<< std::setw(13) << "перевозки" << std::setw(5) << "|" << std::setw(34) << "|\n";
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
		case 1: std::cout << std::setw(10) << "Пешком" << std::setw(4) << "|";
			break;
		case 2:std::cout << std::setw(10) << "Машина" << std::setw(4) << "|";
			break;
		case 3:std::cout << std::setw(10) << "Автобус" << std::setw(4) << "|";
			break;
		case 4:std::cout << std::setw(9) << "Фура" << std::setw(5) << "|";
			break;
		}
		answer = utils::getInt(connection);
		switch (answer) {
		case 1: std::cout << std::setw(16) << "Скоропортящиеся" << std::setw(2) << "|";
			break;
		case 2: std::cout << std::setw(14) << "Продтовары" << std::setw(4) << "|";
			break;
		case 3: std::cout << std::setw(14) << "Промтовары" << std::setw(4) << "|";
			break;
		case 4: std::cout << std::setw(11) << "Люди" << std::setw(7) << "|";
			break;
		}
		count = utils::getInt(connection);
		if (count) {
			int docType = 0;
			for (int j = 0; j < count; j++) {
				docType = utils::getInt(connection);
				switch (docType) {
				case 1: document = "Паспорт"; break;
				case 2: document = "Техпаспорт"; break;
				case 3: document = "Водительское удостоверение"; break;
				case 4: document = "CMR страхование"; break;
				case 5: document = "Счёт-фактура"; break;
				case 6: document = "Упаковочный лист"; break;
				case 7: document = "Сертификат происхождения товара"; break;
				case 8: document = "Транзитная декларация"; break;
				}

				if (j == 0) std::cout << std::setw(32) << document << std::setw(2) << "|\n";
				else if (j == 1 && answer == 1)
					std::cout << " |" << std::setw(11) << "|" << std::setw(27) << "|" << std::setw(14) << "|"
					<< std::setw(14) << "продтовары" << std::setw(4) << "|" << std::setw(32) << document << std::setw(2) << "|\n";
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
	std::cout << "Эксперт 1: по времени прибытия\n\n";
	std::cout.fill('-');
	std::cout << " +" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+"
		<< std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(12) << "+" << std::setw(13) << "+\n";
	std::cout.fill(' ');
	std::cout << " |" << std::setw(8) << "Первый" << std::setw(4) << "|" << std::setw(8) << "Второй" << std::setw(4) << "|"
		<< std::setw(8) << "Третий" << std::setw(4) << "|" << std::setw(10) << "Четвертый" << std::setw(2) << "|"
		<< std::setw(8) << "Пятый" << std::setw(4) << "|" << std::setw(8) << "Шестой" << std::setw(4) << "|"
		<< std::setw(9) << "Седьмой" << std::setw(3) << "|" << std::setw(9) << "Восьмой" << std::setw(3) << "|"
		<< std::setw(9) << "Девятый" << std::setw(3) << "|" << std::setw(9) << "Десятый" << std::setw(3) << "|"
		<< std::setw(10) << "Остальные" << std::setw(3) << "|\n";
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
	std::cout << "Эксперт 2: по способу пересечения и объектам перевозки\n\n";
	std::cout.fill('-');
	std::cout << " +" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+" << std::setw(19) << "+\n";
	std::cout.fill(' ');
	std::cout << " |" << std::setw(12) << "Пешком" << std::setw(7) << "|" << std::setw(12) << "Машина" << std::setw(7) << "|"
		<< std::setw(12) << "Автобус" << std::setw(7) << "|" << std::setw(12) << "Фура со" << std::setw(7) << "|"
		<< std::setw(12) << "Фура с" << std::setw(7) << "|" << std::setw(12) << "Фура с" << std::setw(7) << "|\n";
	std::cout << " |" << std::setw(19) << "|" << std::setw(19) << "|" << std::setw(19) << "|" << std::setw(17) << "скоропортящимися" << std::setw(2) << "|"
		<< std::setw(15) << "продтоварами" << std::setw(4) << "|" << std::setw(15) << "промтоварами" << std::setw(4) << "|\n";
	std::cout << " |" << std::setw(19) << "|" << std::setw(19) << "|" << std::setw(19) << "|"
		<< std::setw(15) << "продтоварами" << std::setw(4) << "|" << std::setw(19) << "|" << std::setw(19) << "|\n";
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
	std::cout << "Документы: \n";
	if (valInt) {
		int count = valInt;
		for (int j = 1; j <= count; j++) {
			std::cout << std::setw(11) << j << ". ";
			valInt = utils::getInt(connection);
			documents.push_back(valInt);
			switch (valInt) {
			case 1: std::cout << "Паспорт\n"; break;
			case 2: std::cout << "Техпаспорт\n"; break;
			case 3: std::cout << "Водительское удостоверение\n"; break;
			case 4: std::cout << "CMR страхование\n"; break;
			case 5: std::cout << "Счёт-фактура\n"; break;
			case 6: std::cout << "Упаковочный лист\n"; break;
			case 7: std::cout << "Сертификат происхождения товара\n"; break;
			case 8: std::cout << "Транзитная декларация\n"; break;
			}
		}
	}
}

void selectingId(SOCKET& connection) {
	while (true) {
		std::cout << "Введите ID нужного пользователя: ";
		int id = 0;
		utils::inputCount(id);
		utils::sendInt(connection, id);
		if (utils::getInt(connection))
			return;
		else
			std::cout << "Введен неверный ID. ";
		if (!utils::YesOrNo("Желаете повторить? y/n ")) {
			utils::sendInt(connection, 0);
			return;
		}
		utils::sendInt(connection, 1);
		std::cout << "\n";
	}
}
