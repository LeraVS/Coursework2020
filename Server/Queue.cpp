#include "Queue.h"

std::vector <std::shared_ptr<User>> Queue::queue = std::vector <std::shared_ptr<User>>();
Queue* Queue::instance = nullptr;

Queue Queue::getInstance() {
	if (!instance)
		instance = new Queue();
	return *instance;
}

Queue::Queue() {}
Queue::~Queue() {}

void Queue::readFromFile() {
	std::ifstream fin("Queue.txt", std::ios_base::binary);
	if (!fin.is_open()) return;
	while (!fin.eof()) {
		if (fin.eof()) return;
		if (!fin) {
			std::cout << "Чтение данных из файла невозможно.\n";
			return;
		}
		User* user = new User();
		fin.read((char*)user, sizeof(User));
		if (fin.eof()) return;
		queue.push_back(std::make_shared<User>(*user));
	}
	fin.close();
	return;
}

bool Queue::writeToFile() {
	std::ofstream fout("Queue.txt", std::ios_base::trunc, std::ios_base::binary);
	if (!fout.is_open()) {
		std::cout << "Файл не может быть открыт.\n";
		return false;
	}
	if (!queue.empty())
		for (auto user : queue) {
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

void Queue::sort() {
	std::sort(queue.begin(), queue.end(), [](std::shared_ptr<User> user1, std::shared_ptr<User> user2) {
		return user1->getNumberInQueue() < user2->getNumberInQueue();
	});
}

void Queue::addUser(std::shared_ptr<User> user) {
	queue.push_back(user);
	this->rankMethod();
	this->sort();
	this->writeToFile();
}

void Queue::removeUser(const int & ID) {
	int number = 0;
	for (auto user : queue) {
		if (user->getID() == ID) break;
		number++;
	}
	queue[number]->editValueOfReguest(RequestToQueueType::NOTSEND);
	queue.erase(queue.begin() + number);

	if (number < queue.size())
		for (auto userIt = queue.begin() + number; userIt != queue.end(); userIt++)
			(*userIt)->editNumberInQueue((*userIt)->getNumberInQueue() - 1);
	this->writeToFile();
}

void Queue::removeUser() {
	queue[0]->passThroughCustoms();
	queue.erase(queue.begin());
	for (auto userIt = queue.begin(); userIt != queue.end(); userIt++)
		(*userIt)->editNumberInQueue((*userIt)->getNumberInQueue() - 1);
	this->writeToFile();
}

bool Queue::containsUser(const int & ID) {
	for (auto user : queue)
		if (user->getID() == ID) return true;
	return false;
}

bool Queue::empty() {
	return queue.empty();
}

std::shared_ptr<User> Queue::getUser(const std::string login) {
	for (auto user : queue) {
		if (user->getLogin() == login)
			return user;
	}
	return nullptr;
}

std::shared_ptr<User> Queue::getUser() {
	return queue[0];
}

const std::vector<std::shared_ptr<User>> Queue::getQueue() {
	return queue;
}

void Queue::rankMethod() {
	int size = this->size();
	float **matrix = new float*[2];
	for (int i = 0; i < 2; i++) {
		matrix[i] = new float[size];
	}

	std::vector<tm> times;
	for (auto user : queue)
		times.push_back(user->getArrivalTime());
	std::sort(times.begin(), times.end());

	int i = 0, j;
	while (i < size) {
		j = 0;
		for (auto user : queue) {
			if (user->getArrivalTime() == times[i]) {
				matrix[0][j] = ExpertRepository::getInstance().assessOnArrivalTime(i + 1);
				i++;
			}
			j++;
		}
	}

	j = 0;
	for (auto user : queue) {
		if (user->getTransport() == TransportType::TRUCK)
			matrix[1][j] = ExpertRepository::getInstance().assessOnTypeOfTransporting(static_cast<int>(user->getTransport()) + static_cast<int>(user->getTransportedCargo()));
		else matrix[1][j] = ExpertRepository::getInstance().assessOnTypeOfTransporting(static_cast<int>(user->getTransport()));
		j++;
	}

	float sumOfLines[2] = { 0, 0 };
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < size; j++)
			sumOfLines[i] += matrix[i][j];

	for (int i = 0; i < 2; i++)
		for (int j = 0; j < size; j++)
			matrix[i][j] = matrix[i][j] / sumOfLines[i];

	float sum, *goalWeights = new float[size];
	for (int j = 0; j < size; j++) {
		sum = 0;
		for (int i = 0; i < 2; i++)
			sum += matrix[i][j];
		goalWeights[j] = sum / 2;
	}
	delete[] matrix;

	float min;
	for (int number = size; number > 0; number--) {
		min = goalWeights[0]; j = 0;
		for (int i = 0; i < size; i++)
			if (goalWeights[i] < min) {
				min = goalWeights[i];
				j = i;
			}
		goalWeights[j] = number;
	}

	j = 0;
	for (auto user : queue) {
		user->editNumberInQueue(goalWeights[j]);
		j++;
	}
}
