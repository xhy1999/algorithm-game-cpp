#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct MapInfo
{
    int** gameMap;
    std::vector<string> path;
};

//һ�ʻ���ں���
void one_line_main(char* map, char* resStr);