#include "UserRepository.h"

std::vector <std::shared_ptr<User>> UserRepository::users = std::vector <std::shared_ptr<User>>();
UserRepository* UserRepository::instance = nullptr;

UserRepository UserRepository::getInstance() {
	if (!instance)
		instance = new UserRepository();
	return *instance;
}

UserRepository::UserRepository() {}
UserRepository::~UserRepository() {}

void UserRepository::readFromFile() {
	std::ifstream fin("UserRepository.txt", std::ios_base::binary);
	if (!fin.is_open()) return;
	while (!fin.eof()) {
		if (!fin) {
			std::cout << "Чтение данных из файла невозможно.\n";
			fin.close();
			return;
		}
		auto user = std::make_shared<User>();
		fin.read((char*)user.get(), sizeof(User));
		if (fin.eof()) {
			fin.close();
			return;
		}
		users.push_back(user);
	}
	fin.close();
}

bool UserRepository::writeToFile() {
	std::ofstream fout("UserRepository.txt", std::ios_base::trunc, std::ios_base::binary);
	if (!fout.is_open()) {
		std::cout << "Файл не может быть открыт.\n";
		return false;
	}
	if (!users.empty())
		for (auto user : users) {
			auto el = dynamic_cast<User*>(user.get());
			fout.write((char*)el, sizeof(User));
			if (!fout) {
				std::cout << "Запись в файл невозможна.\n";
				return false;
			}
		}
	fout.close();
	return true;
}

void UserRepository::addUser(std::shared_ptr<User> user) {
	users.push_back(user);
	this->writeToFile();
}

std::shared_ptr<User> UserRepository::getUser(const std::string login) {
	for (auto user : users) {
		if (user->getLogin() == login)
			return user;
	}
	return nullptr;
}

void UserRepository::removeUser(const int & ID) {
	int number = 0;
	for (auto user : users) {
		if (user->getID() == ID) break;
		number++;
	}
	users.erase(users.begin() + number);
	this->writeToFile();
}

const std::vector<std::shared_ptr<User>> UserRepository::searchRequestToQueue() {
	auto usersWithRequestToQueue = std::vector<std::shared_ptr<User>>();

	for (auto user : users)
		if (user->getValueOfReguest() == RequestToQueueType::NOTREVIEWED)
			usersWithRequestToQueue.push_back(user);

	return usersWithRequestToQueue;
}

bool UserRepository::empty() {
	return users.empty();
}
