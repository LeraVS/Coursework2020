#include "AuthorizationManager.h"


std::vector < std::pair<std::string, std::string>> AuthorizationManager::users = std::vector < std::pair<std::string, std::string>>();
std::pair<std::string, std::string> AuthorizationManager::admin = std::pair<std::string, std::string>();
User* AuthorizationManager::activeUser = nullptr;

AuthorizationManager* AuthorizationManager::instance = nullptr;

AuthorizationManager::AuthorizationManager() {}

AuthorizationManager AuthorizationManager::getInstance() {
	if (!instance)
		instance = new AuthorizationManager();
	return *instance;
}

AuthorizationManager::~AuthorizationManager() {}

bool AuthorizationManager::readFromFileUsers() {
	std::string login, password, count;
	int value = 0;
	std::string line;

	std::ifstream fin("Users.txt", std::ios_base::binary);
	if (!fin.is_open()) return false;

	while (!fin.eof()) {
		std::getline(fin, line, '\n');

		if (fin.eof()) {
			break;
		}

		value++;
		count = utils::useGeek(value);
		login = utils::getTag(count + " login", line);
		char buf[100];
		std::string pas = utils::getTag(count + " password", line);
		strcpy_s(buf, pas.c_str());
		utils::decrypt(buf);
		std::string value = std::string(buf);
		users.push_back(std::make_pair(login, value));
	}
	fin.close();
	return true;
}

bool AuthorizationManager::readFromFileAdmin() {
	std::string line;

	std::ifstream fin("Admin.txt", std::ios_base::binary);
	if (!fin.is_open()) return false;
	while (!fin.eof()) {
		std::getline(fin, line, '\n');
		if (fin.eof()) {
			break;
		}
		admin.first = utils::getTag("login", line);
		char buf[100];
		std::string pas = utils::getTag("password", line);
		strcpy_s(buf, pas.c_str());
		utils::decrypt(buf);
		admin.second = std::string(buf);
	}
	fin.close();
	return true;
}

bool AuthorizationManager::writeToFileUsers() {
	int count = 0;
	std::string value;

	std::ofstream fout("Users.txt", std::ios_base::binary | std::ios_base::trunc);
	if (!fout.is_open()) {
		std::cout << "Файл не может быть открыт.\n";
		return false;
	}
	for (auto user : users) {
		count++;
		value = utils::useGeek(count);

		std::string password = user.second;
		char buf[100];
		strcpy_s(buf, password.c_str());
		utils::encrypt(buf);
		fout << "{" + value + " login=\"" + user.first + "\", " + value + " password=\"" + buf + "\"}\n";
	}
	fout.close();
	return true;
}

bool AuthorizationManager::writeToFileAdmin() {
	std::ofstream fout("Admin.txt", std::ios_base::binary | std::ios_base::trunc);
	if (!fout.is_open()) {
		std::cout << "Файл не может быть открыт.\n";
		return false;
	}

	std::string password = admin.second;
	char buf[100];
	strcpy_s(buf, password.c_str());
	utils::encrypt(buf);
	fout << "{login=\"" + admin.first + "\", password=\"" + buf + "\"}\n";

	fout.close();
	return true;
}

bool AuthorizationManager::authorizationAdmin(std::string login, std::string password) {
	return login == admin.first && password == admin.second;
}

bool AuthorizationManager::authorizationUser(std::string login, std::string password) {
	for (auto& user : users)
		if (login == user.first && password == user.second) {
			auto subjectCopy = dynamic_cast<User*>(UserRepository::getInstance().getUser(login).get());
			activeUser = std::move(subjectCopy);
			return true;
		}
	return false;
}

bool AuthorizationManager::checkAdminLogin() {
	return !admin.first.empty();
}

bool AuthorizationManager::checkUsersLogin(std::string login) {
	for (auto user : users)
		if (user.first == login) return true;
	return false;
}

void AuthorizationManager::setAdmin(std::string login, std::string password) {
	admin.first = login;
	admin.second = password;
	this->writeToFileAdmin();
}

void AuthorizationManager::addUser(std::string login, std::string password) {
	users.push_back(std::make_pair(login, password));
	this->writeToFileUsers();
}

void AuthorizationManager::removeUser(std::string login) {
	int number = 0;
	for (auto user : users) {
		if (user.first == login) break;
		number++;
	}
	users.erase(users.begin() + number);
	this->writeToFileUsers();
}
