#include "Expert.h"

Expert::Expert() {
	scores = std::vector<int>();
}

void Expert::addScore(int score) {
	scores.push_back(score);
}

void Expert::editScore(int ID, int score) {
	scores[ID - 1] = score;
}

void Expert::clear() {
	scores.clear();
}

void Expert::readFromFile(std::ifstream & file) {
	int score;
	scores.clear();

	while (true) {
		if (file.eof()) break;
		if (!file) {
			std::cout << "Чтение данных из файла невозможно.\n";
			return;
		}
		file.read((char*)&score, sizeof(score));
		if (file.eof()) return;
		scores.push_back(score);
	}
}

void Expert::writeToFile(std::ofstream & file) {
	for (auto score : scores) {
		file.write((char*)&score, sizeof(score));
		if (!file) {
			std::cout << "Запись в файл невозможна.\n";
			return;
		}
	}
}

int Expert::getScore(int ID) {
	return scores[ID - 1];
}

Expert::~Expert() {}
