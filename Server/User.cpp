#include "User.h"

User::User() {}

User::User(std::string surname, std::string name, int ID) :
	surname(surname), name(name), ID(ID) {
	requestToQueue = RequestToQueueType::NOTSEND;
	transport = TransportType::ONFOOT;
	transportedCargo = TransportedCargoType::PEOPLE;
	for (int i = 0; i < 8; i++)
		documents[i] = false;
}

void User::editArrivalTime(const tm & time) {
	arrivalTime = time;
}

void User::editDayOfBirth(const tm & date) {
	dayOfBirth = date;
}

void User::editTransport(TransportType transportType) {
	transport = transportType;
}

void User::editTransportedCargo(TransportedCargoType transportedCargoType) {
	transportedCargo = transportedCargoType;
}

void User::addDocument(DocumentType document) {
	documents[(int)document - 1] = true;
}

void User::removeDocument(DocumentType document) {
	documents[(int)document - 1] = false;
}

const std::vector<DocumentType> User::getDocuments() {
	auto docs = std::vector<DocumentType>();
	for (int i = 0; i < 8; i++) {
		if (documents[i])
			docs.push_back(static_cast<DocumentType>(i+1));
	}
	return docs;
}

void User::editValueOfReguest(RequestToQueueType type) {
	requestToQueue = type;
}

void User::editNumberInQueue(int number) {
	numberInQueue = number;
}

void User::passThroughCustoms() {
	arrivalTime.tm_year = 0;
	numberInQueue = 0;
	requestToQueue = RequestToQueueType::NOTSEND;
	for (int i = 0; i < 8; i++)
		documents[i] = false;
}

void User::setLogin(std::string login) {
	this->login = login;
}

User::~User() {}

bool operator<(const User & user1, const User & user2) {
	return user1.numberInQueue < user2.numberInQueue;
}

