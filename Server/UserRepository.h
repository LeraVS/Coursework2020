#pragma once
#include <iostream>
#include <fstream>
#include "User.h"

class UserRepository {
	static std::vector <std::shared_ptr<User>> users;

	static UserRepository* instance;
	UserRepository();
public:
	static UserRepository getInstance();
	~UserRepository();

	void readFromFile();
	bool writeToFile();

	void addUser(std::shared_ptr<User> user);
	std::shared_ptr<User> getUser(const std::string login);
	void removeUser(const int &ID);

	const std::vector <std::shared_ptr<User>> searchRequestToQueue();

	bool empty();
	const int& size() const { return users.size(); }
};

