#pragma once
#include <string>
#include <ctime>
#include <vector>
#include "Enums.h"

class User {
	std::string surname;
	std::string name;
	tm dayOfBirth;
	int ID;

	std::string login;
	TransportType transport;
	TransportedCargoType transportedCargo;
	bool documents[8];
	RequestToQueueType requestToQueue;
	tm arrivalTime;
	int numberInQueue;

public:
	User();
	User(std::string surname, std::string name, int ID);

	std::string getSN() { return name + " " + surname; }
	const int& getID() { return ID; }

	void editDayOfBirth(const tm & date);
	const tm& getDayOfBirth() { return dayOfBirth; }

	void setLogin(std::string login);
	std::string getLogin() { return login; }

	void editTransport(TransportType transportType);
	TransportType getTransport() { return transport; }

	void editTransportedCargo(TransportedCargoType transportedCargoType);
	TransportedCargoType getTransportedCargo() { return transportedCargo; }

	void addDocument(DocumentType document);
	void removeDocument(DocumentType document);
	const std::vector <DocumentType> getDocuments();
	
	void editValueOfReguest(RequestToQueueType type);
	RequestToQueueType getValueOfReguest() { return requestToQueue; }

	void editArrivalTime(const tm & time);
	const tm& getArrivalTime() { return arrivalTime; }

	void editNumberInQueue(int number);
	const int& getNumberInQueue() { return numberInQueue; }

	void passThroughCustoms();

	bool friend operator<(const User& user1, const User& user2);
	
	~User();
};

