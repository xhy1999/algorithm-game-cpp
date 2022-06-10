#include <iostream>
#include <vector>
#include <map>
#include "OneStroke.h"
#include "json/json.h"

using namespace Json;

#define MAP_WIDTH_LENGTH 8
#define MAP_HEIGHT_LENGTH 7

int get_array(int** arr, int var0, int var1) {
    return *(arr[var0] + var1);
}
void set_array(int** arr, int var0, int var1, int value) {
    arr[var0][var1] = value;
}
void print_array(int** arr) {
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            std::cout << std::to_string(get_array(arr, i, j)) + "\t";
        }
        std::cout << std::endl;
    }
}
/*
 * 以下数组宽为2,高为3
 * {{1,2}
 *  {1,2}
 *  {1,2}}
*/
int** init_array(int width, int height) {
    int** arr = new int* [height];
    for (int i = 0; i < height; i++) {
        arr[i] = new int[width];
        for (int j = 0; j < width; j++) {
            arr[i][j] = 0;
        }
    }
    print_array(arr);
    return arr;
}

static int** nextArr = new int*[4] {
    new int[2]{1, 0},
    new int[2]{0, 1},
    new int[2]{-1, 0},
    new int[2]{0, -1}
};

//染色
void dfs(int** dfsMap, int** book, int x, int y, int color) {
    int k, nextX, nextY;
    dfsMap[y][x] = color;
    for (k = 0; k < 4; k++) {
        nextX = x + get_array(nextArr, k, 0);
        nextY = y + get_array(nextArr, k, 1);
        if (nextX < 0 || nextY < 0 || nextY > MAP_HEIGHT_LENGTH - 1 || nextX > MAP_WIDTH_LENGTH - 1) {
            continue;
        }
        if (dfsMap[nextY][nextX] == 0 && book[nextY][nextX] == 0) {
            //sum++;
            book[nextY][nextX] = -1;
            dfs(dfsMap, book, nextX, nextY, color);
        }
    }
}

//获取可以/不可以走到的格子数
int* GetTraversedNum(int** gameMap, int locationX, int locationY) {
    int** dfsMap = gameMap;
    int** book = init_array(MAP_WIDTH_LENGTH, MAP_HEIGHT_LENGTH);
    int sum = 0, num = -10;
    //先染色,计算有几个"岛"
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            if (get_array(dfsMap, i, j) == 0) {
                std::cout << "== 0" << std::endl;
                num--;
                book[i][j] = -1;
                dfs(dfsMap, book, j, i, num);
            }
        }
        std::cout << std::endl;
    }
    print_array(dfsMap);
    map<int, vector<string>> 岛;
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            if (dfsMap[i][j] <= -10) {
                if (岛.count(dfsMap[i][j]) == 0) {
                    岛.insert(dfsMap[i][j], {});
                }
                vector<string> 岛坐标 = 岛[dfsMap[i][j]];
                岛坐标.push_back(j + "," + i);
                岛[dfsMap[i][j]] = 岛坐标;
            }
        }
    }
    std::cout << "123" << std::endl;


    return new int[]{0, 0};
}

MapInfo GO(MapInfo &mapInfo, int startX, int startY, int deep) {
    GetTraversedNum(mapInfo.gameMap, startX, startY);
    return mapInfo;
}

void OneStroke_Main() {
    int** mainMap = new int*[MAP_HEIGHT_LENGTH];
    mainMap[0] = new int[MAP_WIDTH_LENGTH]{ 0, 0, 0, 0, 0, 0, 0, 0 };
    mainMap[1] = new int[MAP_WIDTH_LENGTH]{ 0, 0, 1, 1, 0, 0, 0, 0 };
    mainMap[2] = new int[MAP_WIDTH_LENGTH]{ 0, 1, 1, 0, 0, 0, 0, 0 };
    mainMap[3] = new int[MAP_WIDTH_LENGTH]{ 1, 1, 0, 1, 0, 0, 0, 0 };
    mainMap[4] = new int[MAP_WIDTH_LENGTH]{ 1, 0, 0, 0, 1, 0, 0, 0 };
    mainMap[5] = new int[MAP_WIDTH_LENGTH]{ 0, 0, 0, 0, 0, 0, 0, 0 };
    mainMap[6] = new int[MAP_WIDTH_LENGTH]{ -1, 0, 1, 0, 0, 0, 0, 0 };
    MapInfo mapInfo = { mainMap, {} };
    print_array(mainMap);
    int startX = 0, startY = 6;
    GO(mapInfo, startX, startY, 0);
}

