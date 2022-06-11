#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "OneStroke.h"
#include "PrintUtil.h"

#define MAP_WIDTH_LENGTH 7
#define MAP_HEIGHT_LENGTH 7

int get_array(int** arr, int var0, int var1) {
    return *(arr[var0] + var1);
}
void set_array(int** arr, int var0, int var1, int value) {
    arr[var0][var1] = value;
}
int** copy_array(int** arr) {
    int** mainMap = new int* [MAP_HEIGHT_LENGTH];
    mainMap[0] = new int[MAP_WIDTH_LENGTH];
    mainMap[1] = new int[MAP_WIDTH_LENGTH];
    mainMap[2] = new int[MAP_WIDTH_LENGTH];
    mainMap[3] = new int[MAP_WIDTH_LENGTH];
    mainMap[4] = new int[MAP_WIDTH_LENGTH];
    mainMap[5] = new int[MAP_WIDTH_LENGTH];
    mainMap[6] = new int[MAP_WIDTH_LENGTH];
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            mainMap[i][j] = get_array(arr, i, j);
        }
    }
    return mainMap;
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
    return arr;
}

struct MapInfo
{
    int** gameMap;
    std::vector<string> path;
    MapInfo operator = (MapInfo& info) {
        gameMap = copy_array(info.gameMap);
        path = {};
        copy(info.path.begin(), info.path.end(), inserter(path, path.begin()));
        return *this;
    };
};

string get_string_array(int** arr) {
    string res;
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            res += to_string(get_array(arr, i, j));
        }
    }
    return res;
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

int sum = 0, num = -10;
//获取最多可以走到的格子数
int GetTraversedNum(int** gameMap, int locationX, int locationY) {
    int** dfsMap = copy_array(gameMap);
    int** book = init_array(MAP_WIDTH_LENGTH, MAP_HEIGHT_LENGTH);
    //先染色,计算有几个"岛"
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            if (get_array(dfsMap, i, j) == 0) {
                num--;
                book[i][j] = -1;
                dfs(dfsMap, book, j, i, num);
            }
        }
    }
    //print_array(dfsMap);
    //将岛分类
    map<int, vector<string>> 岛;
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            if (dfsMap[i][j] <= -10) {
                vector<string> 岛坐标 = 岛[dfsMap[i][j]];
                岛坐标.push_back(to_string(j) + "," + to_string(i));
                岛[dfsMap[i][j]] = 岛坐标;
            }
        }
    }
    //print_map(岛);
    //获取下一步能走到的岛
    int maxCanTraversed = 0;
    for (map<int, vector<string>>::iterator iter = 岛.begin(); iter != 岛.end(); iter++) {
        int islandNum = iter->first;
        vector<string> 岛坐标 = iter->second;
        bool canGo = false;
        for (int k = 0; k < 4; k++) {
            int nextX = locationX + nextArr[k][0];
            int nextY = locationY + nextArr[k][1];
            if (std::find(岛坐标.begin(), 岛坐标.end(), to_string(nextX) + "," + to_string(nextY)) != 岛坐标.end()) {
                canGo = true;
                break;
            }
        }
        if (canGo) {
            //根据能走到的岛的面积确定最多能走到格子数量
            int canTraversed = 岛坐标.size();
            if (岛坐标.size() > maxCanTraversed) {
                maxCanTraversed = 岛坐标.size();
            }
        }
    }
    return maxCanTraversed;
}

bool canGoNext(int** map, int nextY, int nextX) {
    if (nextY < 0 || nextX < 0) {
        return false;
    }
    if (nextY > MAP_HEIGHT_LENGTH - 1 || nextX > MAP_WIDTH_LENGTH - 1) {
        return false;
    }
    return map[nextY][nextX] == 0;
}

int GetMaxPathTraversed(int** map) {
    int max = 0;
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            if (map[i][j] == -1) {
                max++;
            }
        }
    }
    return max;
}

static volatile atomic_int maxPathTraversed = 0;
vector<MapInfo> mapInfoList;
set<string> 算过的路径 = {};

void GO(MapInfo &mapInfo, int startY, int startX, int deep) {
    int** oldMap = mapInfo.gameMap;
    int maxCanTraversed = GetTraversedNum(oldMap, startX, startY);
    if (MAP_WIDTH_LENGTH * MAP_HEIGHT_LENGTH - 3 == maxPathTraversed) {
        return;
    }
    //如果当前能到的位置加上,都没有最大值大的话,直接返回
    if (maxPathTraversed >= mapInfo.path.size() + 1 + maxCanTraversed) {
        return;
    }
    string oldMapStr = get_string_array(oldMap) + to_string(startX) + to_string(startY);
    if (算过的路径.find(oldMapStr) != 算过的路径.end()) {
        return;
    }
    算过的路径.insert(oldMapStr);

    MapInfo newMapInfo1 = { NULL, {} }, newMapInfo2 = { NULL, {} }, newMapInfo3 = { NULL, {} }, newMapInfo4 = { NULL, {} };
    bool goNext = false;
    if (canGoNext(oldMap, startY, startX + 1)) {
        int** newMap = copy_array(oldMap);
        newMap[startY][startX + 1] = -1;
        newMapInfo1 = mapInfo;
        newMapInfo1.gameMap = newMap;
        newMapInfo1.path.push_back(to_string(startX + 1) + ", " + to_string(startY));
        GO(newMapInfo1, startY, startX + 1, deep + 1);
        goNext = true;
    }
    if (canGoNext(oldMap, startY + 1, startX)) {
        int** newMap = copy_array(oldMap);
        newMap[startY + 1][startX] = -1;
        newMapInfo2 = mapInfo;
        newMapInfo2.gameMap = newMap;
        newMapInfo2.path.push_back(to_string(startX) + ", " + to_string(startY + 1));
        GO(newMapInfo2, startY + 1, startX, deep + 1);
        goNext = true;
    }
    if (canGoNext(oldMap, startY, startX - 1)) {
        int** newMap = copy_array(oldMap);
        newMap[startY][startX - 1] = -1;
        newMapInfo3 = mapInfo;
        newMapInfo3.gameMap = newMap;
        newMapInfo3.path.push_back(to_string(startX - 1) + ", " + to_string(startY));
        GO(newMapInfo3, startY, startX - 1, deep + 1);
        goNext = true;
    }
    if (canGoNext(oldMap, startY - 1, startX)) {
        int** newMap = copy_array(oldMap);
        newMap[startY - 1][startX] = -1;
        newMapInfo4 = mapInfo;
        newMapInfo4.gameMap = newMap;
        newMapInfo4.path.push_back(to_string(startX) + ", " + to_string(startY - 1));
        GO(newMapInfo4, startY - 1, startX, deep + 1);
        goNext = true;
    }
    if (!goNext) {
        int max = GetMaxPathTraversed(oldMap);
        if (max > maxPathTraversed) {
            maxPathTraversed = max;
            MapInfo saveMapInfo = mapInfo;
            mapInfoList.push_back(saveMapInfo);
        }
    }
}

void one_stroke_main() {
    int** mainMap = new int*[MAP_HEIGHT_LENGTH];
    mainMap[0] = new int[MAP_WIDTH_LENGTH]{ 0, 0, 0, 0, 0, 0, 0 };
    mainMap[1] = new int[MAP_WIDTH_LENGTH]{ 0, 0, 1, 1, 0, 0, 0 };
    mainMap[2] = new int[MAP_WIDTH_LENGTH]{ 0, 1, 1, 0, 0, 0, 0 };
    mainMap[3] = new int[MAP_WIDTH_LENGTH]{ 1, 1, 0, 1, 0, 0, 0 };
    mainMap[4] = new int[MAP_WIDTH_LENGTH]{ 1, 0, 0, 0, 1, 0, 0 };
    mainMap[5] = new int[MAP_WIDTH_LENGTH]{ 0, 0, 0, 0, 0, 0, 0 };
    mainMap[6] = new int[MAP_WIDTH_LENGTH]{ -1, 0, 1, 0, 0, 0, 0 };
    MapInfo mapInfo = { mainMap, {} };
    print_array(mainMap);
    int startX = 0, startY = 6;

    GO(mapInfo, startY, startX, 0);

    std::cout << "算过的路径:" + to_string(算过的路径.size()) << std::endl;
    std::cout << "maxPathTraversed:" + to_string(maxPathTraversed) << std::endl;
    print_vector(mapInfoList[mapInfoList.size() - 1].path);
}

