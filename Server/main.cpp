#define __STDC_WANT_LIB_EXT1__ 1
#include "Utils.h"
#include "Queue.h"
#include "UserRepository.h"
#include "AuthorizationManager.h"

SOCKET Connections[100];
int numberOfConnections = 0;

DWORD WINAPI ThreadFunc(int index);

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

void outputUsers(SOCKET& connection, std::vector <std::shared_ptr<User>> users);
void outputFirstExpertsScores(SOCKET& connection);
void outputSecondExpertsScores(SOCKET& connection);
void outputDocuments(SOCKET& connection);

std::vector <std::shared_ptr<User>> selectingId(SOCKET& connection, std::vector <std::shared_ptr<User>> users);

void readFromFile() {
	AuthorizationManager::getInstance().readFromFileAdmin();
	AuthorizationManager::getInstance().readFromFileUsers();
	UserRepository::getInstance().readFromFile();
	Queue::getInstance().readFromFile();
	ExpertRepository::getInstance().filling();
}
void writeToFile() {
	UserRepository::getInstance().writeToFile();
	Queue::getInstance().writeToFile();
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	readFromFile();

	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		std::cout << "Error\n";
		exit(1);
	}

	std::cout << "Server started.\n";

	SOCKADDR_IN addr;
	int addrSize = sizeof(addr);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1280);
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	bind(sListen, (sockaddr*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET clientSocket;
	for (int i = 0; i < 100; i++) {
		clientSocket = accept(sListen, (sockaddr*)&addr, &addrSize);
		if (clientSocket == 0) std::cout << "Error\n";
		else {
			std::cout << "Client Connected!\n";
			Connections[numberOfConnections] = clientSocket;
			numberOfConnections++;
			DWORD thID;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadFunc, (LPVOID)(i), NULL, &thID);
		}
	}
}

DWORD WINAPI ThreadFunc(int index) {
	bool check = true;
	while (check) {
		switch (utils::getInt(Connections[index])) {
		case 1: enterMenu(Connections[index]); break;
		case 2: addUser(Connections[index]); break;
		case 3: check = false; break;
		}
	}
	std::cout << "Disconnect.\n";
	return 0;
}

bool authorizationAdmin(SOCKET& connection) {
	std::string login, password;
	int i = 3;
	if (!AuthorizationManager::getInstance().checkAdminLogin()) {
		utils::sendInt(connection, 0);
		login = utils::getString(connection);
		password = utils::getString(connection);
		AuthorizationManager::getInstance().setAdmin(login, password);
		return true;
	}
	utils::sendInt(connection, 1);
	while (i) {
		login = utils::getString(connection);
		password = utils::getString(connection);
		if (AuthorizationManager::getInstance().authorizationAdmin(login, password)) {
			utils::sendInt(connection, 1);
			return true;
		}
		utils::sendInt(connection, 0);
		i--;
		if (i == 0) break;
		if (!utils::getInt(connection)) return false;
	}
	return false;
}

bool authorizationUser(SOCKET& connection) {
	std::string login, password;
	int i = 3;
	while (i) {
		login = utils::getString(connection);
		password = utils::getString(connection);
		if (AuthorizationManager::getInstance().authorizationUser(login, password)) {
			utils::sendInt(connection, 1);
			return true;
		}
		utils::sendInt(connection, 0);
		i--;
		if (i == 0) break;
		if (!utils::getInt(connection)) return false;
	}
	return false;
}

void enterMenu(SOCKET& connection) {
	int menu = 0;
	while (true) {
		menu = utils::getInt(connection);
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
		menu = utils::getInt(connection);
		switch (menu) {
		case 1: registeringUserInQueue(connection); break;
		case 2: if (Queue::getInstance().empty()) utils::sendInt(connection, 1);
				else {
			utils::sendInt(connection, 0);
			outputUsers(connection, Queue::getInstance().getQueue());
		}
				break;
		case 3: printingQueue(connection); break;
		case 4: usersPassThroughCustoms(connection); break;
		case 5: editExpertsScores(connection); break;
		case 6: return;
		}
		writeToFile();
	}
}

void userMenu(SOCKET& connection) {
	int menu = 0;
	while (true) {
		menu = utils::getInt(connection);
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
		writeToFile();
	}
}

void requestToQueueMenu(SOCKET& connection) {
	int menu = utils::getInt(connection);
	switch (menu) {
	case 1: sendingRequest(connection); break;
	case 2: requestVerification(connection); break;
	case 3: return;
	}
}

void addUser(SOCKET& connection) {
	std::string login, password;

	std::string surname, name;
	int ID, answer = 0;
	tm dayOfBirth;

	surname = utils::getString(connection);
	name = utils::getString(connection);
	ID = utils::getInt(connection);
	dayOfBirth.tm_mday = utils::getInt(connection);
	dayOfBirth.tm_mon = utils::getInt(connection);
	dayOfBirth.tm_year = utils::getInt(connection);

	User user(surname, name, ID);
	user.editDayOfBirth(dayOfBirth);

	answer = utils::getInt(connection);
	switch (answer) {
	case 1: user.editTransport(TransportType::ONFOOT); break;
	case 2: user.editTransport(TransportType::CAR); break;
	case 3: user.editTransport(TransportType::BUS); break;
	case 4: user.editTransport(TransportType::TRUCK); break;
	}
	if (answer == 4) {
		answer = utils::getInt(connection);
		switch (answer) {
		case 1: user.editTransportedCargo(TransportedCargoType::PERISHABLEEDIBLE); break;
		case 2: user.editTransportedCargo(TransportedCargoType::EDIBLE); break;
		case 3: user.editTransportedCargo(TransportedCargoType::MANUFACTUREDGOODS); break;
		}
	}

	while (true) {
		login = utils::getString(connection);
		if (!AuthorizationManager::getInstance().checkUsersLogin(login)) {
			utils::sendInt(connection, 0);
			break;
		}
		utils::sendInt(connection, 1);
	}
	password = utils::getString(connection);
	user.setLogin(login);
	AuthorizationManager::getInstance().addUser(login, password);
	UserRepository::getInstance().addUser(std::make_shared<User>(user));
}

void addDocuments(SOCKET& connection) {
	auto activeUser = AuthorizationManager::getInstance().currentUser();
	if (activeUser->getValueOfReguest() == RequestToQueueType::ACCEPTED) {
		utils::sendInt(connection, 1);
		return;
	} else utils::sendInt(connection, 0);
	int answer = 0;
	utils::sendInt(connection, static_cast<int>(activeUser->getTransport()));
	while (true) {
		if (!activeUser->getDocuments().empty()) {
			utils::sendInt(connection, 0);
			outputDocuments(connection);
		} else utils::sendInt(connection, 1);
		if (utils::getInt(connection)) {
			answer = utils::getInt(connection);
			switch (answer) {
			case 1: activeUser->addDocument(DocumentType::PASSPORT); break;
			case 2: activeUser->addDocument(DocumentType::TECHPASSPORT); break;
			case 3: activeUser->addDocument(DocumentType::DRIVERSLICENSE); break;
			case 4: activeUser->addDocument(DocumentType::CMRINSURANCE); break;
			case 5: activeUser->addDocument(DocumentType::INVOICE); break;
			case 6: activeUser->addDocument(DocumentType::PACKINGLIST); break;
			case 7: activeUser->addDocument(DocumentType::CERTIFICATEOFORIGIN); break;
			case 8: activeUser->addDocument(DocumentType::TRANSITDECLARATION); break;
			}
		}
		if (!utils::getInt(connection)) return;
	}
}

bool removeUser(SOCKET& connection) {
	auto activeUser = AuthorizationManager::getInstance().currentUser();
	if (utils::getInt(connection)) {
		if (Queue::getInstance().containsUser(activeUser->getID()))
			Queue::getInstance().removeUser(activeUser->getID());
		AuthorizationManager::getInstance().removeUser(activeUser->getLogin());
		UserRepository::getInstance().removeUser(activeUser->getID());
		return true;
	}
	return false;
}

void registeringUserInQueue(SOCKET& connection) {
	auto usersWithRequestToQueue = UserRepository::getInstance().searchRequestToQueue();
	if (usersWithRequestToQueue.empty()) {
		utils::sendInt(connection, 1);
		return;
	}
	utils::sendInt(connection, 0);
	outputUsers(connection, usersWithRequestToQueue);

	auto selectedUser = selectingId(connection, usersWithRequestToQueue);
	if (selectedUser.empty()) {
		utils::sendInt(connection, 1);
		return;
	}
	utils::sendInt(connection, 0);
	outputUsers(connection, selectedUser);

	if (utils::getInt(connection)) {
		UserRepository::getInstance().getUser(selectedUser[0]->getLogin())->editValueOfReguest(RequestToQueueType::ACCEPTED);
		Queue::getInstance().addUser(UserRepository::getInstance().getUser(selectedUser[0]->getLogin()));
	} else UserRepository::getInstance().getUser(selectedUser[0]->getLogin())->editValueOfReguest(RequestToQueueType::REJECTED);
}

void printingQueue(SOCKET& connection) {
	if (Queue::getInstance().empty()) {
		utils::sendInt(connection, 1);
		return;
	}
	utils::sendInt(connection, 0);
	if (!utils::getInt(connection)) {
		return;
	}
	auto users = Queue::getInstance().getQueue();
	utils::sendInt(connection, users.size());

	std::vector <DocumentType> documents;
	for (auto user : users) {
		utils::sendInt(connection, user->getID());
		utils::sendString(connection, user->getSN());
		utils::sendInt(connection, static_cast<int>(user->getTransport()));
		utils::sendInt(connection, static_cast<int>(user->getTransportedCargo()) + 1);
		documents = user->getDocuments();
		utils::sendInt(connection, documents.size());
		if (!documents.empty()) {
			for (auto document : documents)
				utils::sendInt(connection, static_cast<int>(document));
		}
		if (!utils::getInt(connection)) {
			return;
		}
	}
}

void usersPassThroughCustoms(SOCKET& connection) {
	if (Queue::getInstance().empty()) {
		utils::sendInt(connection, 1);
		return;
	}
	utils::sendInt(connection, 0);
	auto userForPassThroughCustoms = std::vector<std::shared_ptr<User>>();
	userForPassThroughCustoms.push_back(Queue::getInstance().getUser());
	outputUsers(connection, userForPassThroughCustoms);
	Queue::getInstance().removeUser();
}

void personalAccount(SOCKET& connection) {
	auto activeUser = AuthorizationManager::getInstance().currentUser();
	utils::sendString(connection, activeUser->getSN());
	utils::sendInt(connection, activeUser->getDayOfBirth().tm_mday);
	utils::sendInt(connection, activeUser->getDayOfBirth().tm_mon);
	utils::sendInt(connection, activeUser->getDayOfBirth().tm_year);
	utils::sendInt(connection, activeUser->getID());
	utils::sendInt(connection, static_cast<int>(activeUser->getTransport()));
	if (activeUser->getTransport() == TransportType::TRUCK)
		utils::sendInt(connection, static_cast<int>(activeUser->getTransportedCargo()) + 1);
	std::vector <DocumentType> documents = activeUser->getDocuments();
	utils::sendInt(connection, documents.size());
	if (!documents.empty()) {
		for (auto document : documents)
			utils::sendInt(connection, static_cast<int>(document));
	}
}

void sendingRequest(SOCKET& connection) {
	auto activeUser = AuthorizationManager::getInstance().currentUser();

	if (activeUser->getDocuments().empty()) {
		utils::sendInt(connection, 1);
		return;
	} else utils::sendInt(connection, 0);

	utils::sendInt(connection, static_cast<int>(activeUser->getValueOfReguest()));
	switch (activeUser->getValueOfReguest()) {
	case RequestToQueueType::NOTREVIEWED: return;
	case RequestToQueueType::ACCEPTED: return;
	}

	tm arrivalTime;
	time_t seconds = time(NULL);
	gmtime_s(&arrivalTime, &seconds);
	activeUser->editArrivalTime(arrivalTime);

	activeUser->editValueOfReguest(RequestToQueueType::NOTREVIEWED);
	utils::sendInt(connection, 1);
}

void requestVerification(SOCKET& connection) {
	auto activeUser = AuthorizationManager::getInstance().currentUser();
	utils::sendInt(connection, static_cast<int>(activeUser->getValueOfReguest()));
	if (activeUser->getValueOfReguest() == RequestToQueueType::ACCEPTED)
		utils::sendInt(connection, activeUser->getNumberInQueue());
}

void editExpertsScores(SOCKET& connection) {
	int score = 0, menu = 0, size = 0;
	auto scores = std::vector<int>();
	menu = utils::getInt(connection);
	switch (menu) {
	case 1:
		outputFirstExpertsScores(connection);
		for (int i = 0; i < 11; i++) {
			score = utils::getInt(connection);
			scores.push_back(score);
		}
		ExpertRepository::getInstance().editScoresOfExpertOfArrivalTime(scores);
		break;
	case 2:
		outputSecondExpertsScores(connection);
		for (int i = 0; i < 6; i++) {
			score = utils::getInt(connection);
			scores.push_back(score);
		}
		ExpertRepository::getInstance().editScoresOfExpertOnTypeOfTransporting(scores);
		break;
	case 3: return;
	}
	
	Queue::getInstance().rankMethod();
	Queue::getInstance().sort();
}

void editData(SOCKET& connection) {
	auto activeUser = AuthorizationManager::getInstance().currentUser();
	if (activeUser->getValueOfReguest() == RequestToQueueType::ACCEPTED) {
		utils::sendInt(connection, 1);
		return;
	} else utils::sendInt(connection, 0);
	int type = 0;
	int menu = utils::getInt(connection);
	switch (menu) {
	case 1:
	{
		utils::sendInt(connection, static_cast<int>(activeUser->getTransport()));
		type = utils::getInt(connection);
		switch (type) {
		case 1: activeUser->editTransport(TransportType::ONFOOT); break;
		case 2: activeUser->editTransport(TransportType::CAR); break;
		case 3: activeUser->editTransport(TransportType::BUS); break;
		case 4: activeUser->editTransport(TransportType::TRUCK); break;
		}
		if (type == 4) {
			type = utils::getInt(connection);
			switch (type) {
			case 1: activeUser->editTransportedCargo(TransportedCargoType::PERISHABLEEDIBLE); break;
			case 2: activeUser->editTransportedCargo(TransportedCargoType::EDIBLE); break;
			case 3: activeUser->editTransportedCargo(TransportedCargoType::MANUFACTUREDGOODS); break;
			}
		}
	}
	break;
	case 2:
	{
		utils::sendInt(connection, static_cast<int>(activeUser->getTransport()));
		if (activeUser->getTransport() != TransportType::TRUCK) return;
		utils::sendInt(connection, static_cast<int>(activeUser->getTransportedCargo()) + 1);
		type = utils::getInt(connection);
		switch (type) {
		case 1: activeUser->editTransportedCargo(TransportedCargoType::PERISHABLEEDIBLE); break;
		case 2: activeUser->editTransportedCargo(TransportedCargoType::EDIBLE); break;
		case 3: activeUser->editTransportedCargo(TransportedCargoType::MANUFACTUREDGOODS); break;
		}
	}
	break;
	case 3: return;
	}
}

void outputUsers(SOCKET& connection, std::vector <std::shared_ptr<User>> users) {
	utils::sendInt(connection, users.size());

	std::vector <DocumentType> documents;
	for (auto user : users) {
		utils::sendInt(connection, user->getID());
		utils::sendString(connection, user->getSN());
		utils::sendInt(connection, static_cast<int>(user->getTransport()));
		utils::sendInt(connection, static_cast<int>(user->getTransportedCargo()) + 1);
		documents = user->getDocuments();
		utils::sendInt(connection, documents.size());
		if (!documents.empty()) {
			for (auto document : documents)
				utils::sendInt(connection, static_cast<int>(document));
		}
	}
}

void outputFirstExpertsScores(SOCKET& connection) {
	auto scores = ExpertRepository::getInstance().getScoresOfExpertOfArrivalTime();
	for (auto score : scores)
		utils::sendInt(connection, score);
}

void outputSecondExpertsScores(SOCKET& connection) {
	auto scores = ExpertRepository::getInstance().getScoresOfExpertOnTypeOfTransporting();
	for (auto score : scores)
		utils::sendInt(connection, score);
}

void outputDocuments(SOCKET& connection) {
	std::vector <DocumentType> documents = AuthorizationManager::getInstance().currentUser()->getDocuments();
	utils::sendInt(connection, documents.size());
	if (!documents.empty()) {
		for (auto document : documents)
			utils::sendInt(connection, static_cast<int>(document));
	}
}

std::vector<std::shared_ptr<User>> selectingId(SOCKET& connection, std::vector<std::shared_ptr<User>> users) {
	auto selectedUser = std::vector<std::shared_ptr<User>>();
	while (true) {
		int id = utils::getInt(connection);
		for (auto user : users) {
			if (user->getID() == id) {
				selectedUser.push_back(user);
				utils::sendInt(connection, 1);
				return selectedUser;
			}
		}
		utils::sendInt(connection, 0);
		if (!utils::getInt(connection)) return selectedUser;
	}
}
