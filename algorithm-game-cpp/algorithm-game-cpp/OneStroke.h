#pragma once
#include <vector>
#include <string>

using namespace std;

struct MapInfo
{
	int** gameMap;
	std::vector<string> path;
};

void OneStroke_Main();