#pragma once
#include "Expert.h"

class ExpertRepository {
	static Expert expertOfArrivalTime;
	static Expert expertOnTypeOfTransporting;

	static ExpertRepository* instance;
	ExpertRepository();

public:
	static ExpertRepository getInstance();
	~ExpertRepository();

	void filling();

	void editScoresOfExpertOfArrivalTime(std::vector<int> scores); 
	void editScoresOfExpertOnTypeOfTransporting(std::vector<int> scores);

	int assessOnArrivalTime(int ID);
	int assessOnTypeOfTransporting(int ID);

	const std::vector<int> getScoresOfExpertOfArrivalTime() { return expertOfArrivalTime.getScores(); }
	const std::vector<int> getScoresOfExpertOnTypeOfTransporting() { return expertOnTypeOfTransporting.getScores(); }
};
