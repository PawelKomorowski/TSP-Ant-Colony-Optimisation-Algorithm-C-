#pragma once
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <random>
#include <cmath>
class Ant
{
	int pathLong;// Path size
	int *path;// Ant path array
	bool *visited;// Visited cities array. False, when city was not visited
public:
	Ant(int n);
	~Ant();
	int getPathLong();// Return path size
	void clear();// Clear visited array
	int* getPath();// Return ant path array
	void updatePath(int i, int city);// Update path: set city at given index
	bool* getVisited();// Return visited city array
	void updateVisited(int i);// Update visited array: set true at given index
};

