#include "ExpertRepository.h"

Expert ExpertRepository::expertOfArrivalTime = Expert();
Expert ExpertRepository::expertOnTypeOfTransporting = Expert();
ExpertRepository* ExpertRepository::instance = nullptr;

ExpertRepository ExpertRepository::getInstance() {
	if (!instance)
		instance = new ExpertRepository();
	return *instance;
}

ExpertRepository::ExpertRepository() {}
ExpertRepository::~ExpertRepository() {}

void ExpertRepository::filling() {
	std::ifstream file;
	file.open("ExpertOfArrivalTime.txt", std::ios::binary);
	if (!file) {
		std::cout << "Невозможно открыть файл.\n";
		for (int i = 10; i >= 0; i--)
			expertOfArrivalTime.addScore(i);
	} else if (file.eof())
		for (int i = 10; i >= 0; i--)
			expertOfArrivalTime.addScore(i);
	else expertOfArrivalTime.readFromFile(file);
	file.close();

	file.open("ExpertOnTypeOfTransporting.txt", std::ios::binary);
	if (!file) {
		std::cout << "Невозможно открыть файл.\n";
		expertOnTypeOfTransporting.addScore(9);
		expertOnTypeOfTransporting.addScore(5);
		expertOnTypeOfTransporting.addScore(10);
		expertOnTypeOfTransporting.addScore(7);
		expertOnTypeOfTransporting.addScore(3);
		expertOnTypeOfTransporting.addScore(1);
	} else if (file.eof()) {
		expertOnTypeOfTransporting.addScore(9);
		expertOnTypeOfTransporting.addScore(5);
		expertOnTypeOfTransporting.addScore(10);
		expertOnTypeOfTransporting.addScore(7);
		expertOnTypeOfTransporting.addScore(3);
		expertOnTypeOfTransporting.addScore(1);
	} else expertOnTypeOfTransporting.readFromFile(file);
	file.close();
}

void ExpertRepository::editScoresOfExpertOfArrivalTime(std::vector<int> scores) {
	expertOfArrivalTime.clear();
	for (auto score : scores)
		expertOfArrivalTime.addScore(score);

	std::ofstream file;
	file.open("ExpertOfArrivalTime.txt", std::ios::trunc | std::ios::binary);
	if (!file) {
		std::cout << "Невозможно открыть файл.\n";
		return;
	}
	expertOfArrivalTime.writeToFile(file);
	file.close();
}

void ExpertRepository::editScoresOfExpertOnTypeOfTransporting(std::vector<int> scores) {
	expertOnTypeOfTransporting.clear();
	for (auto score : scores)
		expertOnTypeOfTransporting.addScore(score);

	std::ofstream file;
	file.open("ExpertOnTypeOfTransporting.txt", std::ios::trunc | std::ios::binary);
	if (!file) {
		std::cout << "Невозможно открыть файл.\n";
		return;
	}
	expertOnTypeOfTransporting.writeToFile(file);
	file.close();
}

int ExpertRepository::assessOnArrivalTime(int ID) {
	return expertOfArrivalTime.getScore(ID);
}

int ExpertRepository::assessOnTypeOfTransporting(int ID) {
	return expertOnTypeOfTransporting.getScore(ID);
}

