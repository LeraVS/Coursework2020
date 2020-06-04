#pragma once
#include "Utils.h"
#include "UserRepository.h"

class AuthorizationManager {
	static std::vector < std::pair<std::string, std::string>> users;
	static std::pair<std::string, std::string> admin;

	static User* activeUser;

	static AuthorizationManager *instance;
	AuthorizationManager();

public:

	static AuthorizationManager getInstance();
	~AuthorizationManager();

	bool readFromFileUsers();
	bool readFromFileAdmin();
	bool writeToFileUsers();
	bool writeToFileAdmin();

	bool authorizationAdmin(std::string login, std::string password);
	bool authorizationUser(std::string login, std::string password);
	bool checkUsersLogin(std::string login);
	bool checkAdminLogin();

	void setAdmin(std::string login, std::string password);
	void addUser(std::string login, std::string password);
	void removeUser(std::string login);
	User* currentUser() { return activeUser; }
};

