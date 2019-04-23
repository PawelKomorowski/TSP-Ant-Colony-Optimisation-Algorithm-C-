#include "pch.h"
#include "Ant.h"


Ant::Ant(int n)
{
	pathLong = n;
	this->path = new int[n];
	this->visited = new bool[n];
}

Ant::~Ant()
{
	pathLong = -1;
	delete[]path;
	delete[]visited;
}

int Ant::getPathLong()
{
	return this->pathLong;
}

void Ant::clear()
{
	for (int i = 0; i < pathLong; i++)
		visited[i] = false;
}

int * Ant::getPath()
{
	return path;
}

void Ant::updatePath(int i, int city)
{
	this->path[i] = city;
}

bool * Ant::getVisited()
{
	return visited;
}

void Ant::updateVisited(int i)
{
	this->visited[i] = true;
}



