#pragma once
#include "Ant.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <random>
#include <cmath>
//Before start algorith all parameters must be determinate: alpha, beta, ro, antsSize(population size)
class Aco
{
	int** graph;// Graph which contains cities
	int size;// Graph size
	int weight;// Shortest path weight(length) 
	int *path;// Shortest path
	float **pheromone;// Pheromone array is the same matrix as graph but instead of weights between vertexes there is pheromone wihich ants putting down.
					  // It is using to calculate probability that ant can go to another city
	int antsSize; // Ants amount
	Ant **ants;// Ants which will try to find path
	int antPheromone;// One ant pheromone
	float ro;// Evaporation rate
	int alpha, beta;// Exponent used to calculate probability
	int time;// Algorithm work time in seconds
public:
	Aco(string file_name);// Read date from TSTP text file. Exaplary file are in folder data
	~Aco();
	void printGraph();// Print grapf matrix
	void printPheromone();// Print pheromone matrix
	void printPath(int* path);// Print path
	int getWeight();// Return path weight
	int* getPath();// Return path
	void setAntPheromone(int pheromone);// Set pheromone for one ant
	void setRo(float ro);// Set evaporation rate
	void setAlpha(int alpha);// Set alpha exponent
	void setBeta(int beta);// Set beta exponent
	void setAntsSize(int m);// Set ants amount
	void initAnts();// Create ants
	void initPheromon();// Initialize feromone value acording to Dorigio: m/Cnn (ants amount/supposed minimal path calculate by simply heuristic)
	int minimal_path();// Kalculate supposed minilam path(Cnn) by heuristic
	int pathLength(int* path);// Kalculate given path weigth
	void evaporate();// Update pheromone: evaporate pheromone and then add it using paths found by ants(ant-cycle CAS algorithm)
	int next_city(bool* visited, int city);// Return next city acorging to visited cities and curren ant city using probability
	double random_d_0_1(); // Return rendom number from 0 to 1 which will be comared with calculated probability
	int random_i_0_size(); // Return random number from 0 to size-1, which will be uesd as next city in the path when probability will not accept any city as next city
	bool uni_path_behavior();// Chech if all ants paths are not the same. In case they are return true
	bool comparePaths(int* path1, int* path2);// Comare two paths and return true it they are the same
	void antColonyOptimization();// Start anth colony optimalization algorithm
	void clearAnts();// Clear all ants memory(visited city arrays)
	void setPath(int* path);// Set currently the shortest path if ant find id
	void setWeight(int weight);// Set currently the shortest path weight(length)
	int* antsShortestPath();// Return shortest path which ants find in one algorithm circle
	void setTime(int time);// Set time works algorithm
	int getSize();// Get graph size
};

