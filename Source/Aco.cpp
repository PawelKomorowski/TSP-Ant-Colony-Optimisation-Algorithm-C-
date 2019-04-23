#include "pch.h"
#include "Aco.h"
#include "Ant.h"
using namespace std;
using tp = std::chrono::time_point<std::chrono::system_clock>;
using duration = std::chrono::duration<float>;
Aco::Aco(string file_name)
{
	fstream file;
	file.open(file_name, ios::in);
	string pomin;
	int m;
	file >> pomin;
	file >> this->size;
	file >> pomin;
	file >> m;
	getline(file, pomin);
	getline(file, pomin);
	getline(file, pomin);
	for (int i = 0; i < m; i++)
		getline(file, pomin);
	this->path = new int[this->size];
	this->graph = new int*[size];
	this->pheromone = new float*[size];
	for (int i = 0; i < size; i++) {
		this->graph[i] = new int[size];
		this->pheromone[i] = new float[size];
	}
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			file >> graph[i][j];
	this->weight = INT32_MAX;
}

Aco::~Aco()
{
	for (int i = 0; i < this->size; i++) {
		delete[] this->graph[i];
		delete[] this->pheromone[i];
	}
	delete[] this->graph;
	delete[] this->pheromone;
	delete[] this->path;
	delete[] this->ants;
}

void Aco::printGraph()
{
	cout.width(5);
	cout << internal << " ";
	for (int i = 0; i < this->size; i++) {
		cout.width(5);
		cout << internal << i;
	}
	cout << endl;
	for (int i = 0; i < this->size; i++) {
		cout.width(5);
		cout << internal << i;
		for (int j = 0; j < this->size; j++) {
			cout.width(5);
			cout << internal << this->graph[i][j];
		}
		cout << endl;
	}
}

void Aco::printPheromone()
{
	cout.width(5);
	cout << internal << " ";
	for (int i = 0; i < this->size; i++) {
		cout.width(5);
		cout << internal << i;
	}
	cout << endl;
	for (int i = 0; i < this->size; i++) {
		cout.width(5);
		cout << internal << i;
		for (int j = 0; j < this->size; j++) {
			cout.width(5);
			cout << internal << this->pheromone[i][j];
		}
		cout << endl;
	}
}

void Aco::printPath(int * path)
{
	for (int i = 0; i < this->size; i++)
		cout << path[i] << " ";
	cout << endl;
}

int Aco::getWeight()
{
	return this->weight;
}

int * Aco::getPath()
{
	return this->path;
}

void Aco::setAntPheromone(int pheromone)
{
	this->antPheromone = pheromone;
}

void Aco::setRo(float ro)
{
	this->ro = ro;
}

void Aco::setAlpha(int alpha)
{
	this->alpha = alpha;
}

void Aco::setBeta(int beta)
{
	this->beta = beta;
}

void Aco::setAntsSize(int m)
{
	this->antsSize = m;
}

void Aco::initAnts()
{
	this->ants = new Ant*[this->antsSize];
	for (int i = 0; i < this->antsSize; i++)
		ants[i] = new Ant(this->size);
}

void Aco::initPheromon()
{
	float initVal = (float)size / (float)minimal_path();
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (i == j)
				this->pheromone[i][j] = 0;
			else
				this->pheromone[i][j] = initVal;
}

int Aco::minimal_path()
{
	int min = 0, value = 0;
	for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (i != j) {
					if (min == 0)
						min = graph[i][j];
					else
						if (graph[i][j] < min)
							min = graph[i][j];
				}
			}
		value += min;
		min = 0;
	}
	return value;
}

int Aco::pathLength(int * path)
{
	int length = 0;
	for (int i = 0; i < size - 1; i++)
		length += graph[path[i]][path[i + 1]];
	length += graph[path[size - 1]][path[0]];
	return length;
}

void Aco::evaporate()// Update pheromone
{
	for(int i = 0; i < size; i++)// Evaporate pheromone
		for (int j = 0; j < size; j++)
			pheromone[i][j] *= ro;

	int* tempPath;
	float updateVall;
	for (int i = 0; i < antsSize; i++) {// Iterate ants
		tempPath = ants[i][0].getPath();// Ant i path
		updateVall = (float)antPheromone / (float)pathLength(tempPath);// Pheromone value which will be put to pheromone array accorging to CAS
		for (int j = 0; j < size - 1; j++)// Iterate path
			pheromone[tempPath[j]][tempPath[j + 1]] += updateVall;// Add pheromone
		pheromone[tempPath[size - 1]][tempPath[0]] += updateVall;
	}
}

int Aco::next_city(bool * visited, int city)// Return not visited city with caltulated probability
{
	double licznik = 0, mianownik = 0, pstwo;
	for (int j = 0; j < size; j++) {// Sum of pheromone and accessibility of not visited city from ant current city
		if (!visited[j] && j != city) {// If city was not visited
			mianownik += pow(pheromone[city][j], alpha)*pow(1.0 / (float)graph[city][j], beta); // Calculate denominatod of probability function
		}
	}
	for (int i = 0; i < size; i++) {// Calculate probability for another city. If no city was chose then next city will be choose randomly
		if (!visited[i] && i != city) {
			licznik = pow(pheromone[city][i], alpha)*pow(1.0 / (float)graph[city][i], beta);// Calculate nominator probability function for not visited(accessible) city
			pstwo = licznik / mianownik;// Calculate probability and choose next city
			if (random_d_0_1() <= pstwo) // If probability is bigger than random number then return taht city. It will be next city at ant path
				return i;
		}
	}
	int random_city;
	do {// If any city was choosen by probability then return random not visited city
		random_city = random_i_0_size();// Random city
	} while (visited[random_city]);// Draw next city until not visited

	return random_city;// Return random city
}

double Aco::random_d_0_1()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dis(0.0, 1.0);
	return dis(gen);
}

int Aco::random_i_0_size()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, this->size - 1);
	return dis(gen);
}

bool Aco::uni_path_behavior()
{
	for (int i = 0; i < antsSize - 1; i++)
		if (!comparePaths(ants[i][0].getPath(), ants[i + 1][0].getPath()))// If 2 alternate paths are different then there is not uni path behavior case and return false
			return false;
	return true;// All paths are the same and return true
}

bool Aco::comparePaths(int * path1, int * path2)
{
	for (int i = 0; i < size; i++)
		if (path1[i] != path2[i])
			return false;
	return true;
}

void Aco::antColonyOptimization()
{
	setWeight(INT32_MAX);// Initialize shortest path weight(length) as big number
	initPheromon();// Initialize pheromone on graf paths
	initAnts();// Create ants
	tp start;// Variable to clock algorithm
	duration dur;
	start = std::chrono::system_clock::now();// Start clock
	int city;// Ant next city. Diuring choosing next city it works as ant current city.
	do {
		clearAnts();// Clear ants memory(visited city arrays)
		// Calculate path for each ant
		for (int i = 0; i < antsSize; i++) {// Iterate ants
			for (int j = 0; j < size; j++) {// Iterate cities for each ant
				if (j == 0) {// First city is choosen randomly
					city = random_i_0_size();// Draw city
					ants[i][0].updatePath(0, city);// Update ant path
					ants[i][0].updateVisited(city);// Update visited city array
				}
				else {// Choose another city 
					city = next_city(ants[i][0].getVisited(), city);// Choose next city by probability fincion
					ants[i][0].updatePath(j, city);// Update ant path
					ants[i][0].updateVisited(city);// Update visited city array
				}
			}
		}
		// Now each ant has path
		evaporate();// Update pheromone
		int* tempPath = antsShortestPath();// Get shortest path which was found by ants in this(one) iterarion
		int tempWeight = pathLength(tempPath);// Get the shortest path weight(length)
		if (tempWeight < this->weight)// If found path is shortest than the shortest found from begining(shorstet global path for algorithm) then update algorithm path(best solution)
		{
			this->weight = tempWeight;
			setPath(tempPath);
		}
		dur = std::chrono::system_clock::now() - start;// Calculate algorithm working time
	} while ((int)dur.count() < time && !uni_path_behavior());// If time is ovre or there is uni path behavioral case then end algorintk
}

void Aco::clearAnts()
{
	for (int i = 0; i < antsSize; i++)
		ants[i][0].clear();
}

void Aco::setPath(int * path)
{
	for (int i = 0; i < size; i++)
		this->path[i] = path[i];
}

void Aco::setWeight(int weight)
{
	this->weight = weight;
}

int * Aco::antsShortestPath()
{
	int pathWeight = INT32_MAX;
	int tempWeight;
	int antIndex;
	for (int i = 0; i < antsSize; i++) {
		tempWeight = pathLength(ants[i][0].getPath());
		if (pathWeight > tempWeight) {
			pathWeight = tempWeight;
			antIndex = i;
		}
	}
	return ants[antIndex][0].getPath();
}

void Aco::setTime(int time)
{
	this->time = time;
}

int Aco::getSize()
{
	return this->size;
}