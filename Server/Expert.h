#pragma once
#include <iostream>
#include <fstream>
#include <vector>

class Expert {
protected:
	std::vector <int> scores;

public:
	Expert();

	void addScore(int score);
	void editScore(int ID, int score);
	int size() { return scores.size(); }
	void clear();

	void readFromFile(std::ifstream& file);
	void writeToFile(std::ofstream& file);

	int getScore(int ID);
	const std::vector <int> getScores() { return scores; }

	~Expert();
};

