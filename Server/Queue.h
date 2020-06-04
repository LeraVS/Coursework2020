#pragma once
#include "Utils.h"
#include "User.h"
#include "ExpertRepository.h"

class Queue {
	static std::vector <std::shared_ptr<User>> queue;

	static Queue* instance;
	Queue();
public:
	static Queue getInstance();
	~Queue();

	void readFromFile();
	bool writeToFile();

	void sort();

	void addUser(std::shared_ptr<User> user);
	std::shared_ptr<User> getUser(const std::string login);
	std::shared_ptr<User> getUser();
	void removeUser(const int &ID);
	void removeUser();

	bool containsUser(const int &ID);

	bool empty();
	const int& size() const { return queue.size(); }

	const std::vector <std::shared_ptr<User>> getQueue();

	void rankMethod();
};

