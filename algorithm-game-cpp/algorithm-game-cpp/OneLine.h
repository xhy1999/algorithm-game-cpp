#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct MapInfo
{
    int** gameMap;
    std::vector<string> path;
};

//一笔画入口函数
void one_line_main(char* map, char* resStr);