#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <set>
#include <string>
#include "ArrayUtils.h"
#include "MapUtils.h"
#include "SetUtils.h"
#include "StringUtils.h"
#include "VectorUtils.h"

using namespace std;

struct MapInfo
{
    int** gameMap;
    std::vector<string> path;
};

static int** nextArr = new int* [4]
{
    new int[2] {1, 0},
    new int[2] {0, 1},
    new int[2] {-1, 0},
    new int[2] {0, -1}
};

//染色
void dfs(int** dfsMap, int** book, int x, int y, int color) {
    int k, nextX, nextY;
    dfsMap[y][x] = color;
    for (k = 0; k < 4; k++) {
        nextX = x + array_get(nextArr, k, 0);
        nextY = y + array_get(nextArr, k, 1);
        if (nextX < 0 || nextY < 0 || nextY > MAP_HEIGHT_LENGTH - 1 || nextX > MAP_WIDTH_LENGTH - 1) {
            continue;
        }
        if (dfsMap[nextY][nextX] == 0 && book[nextY][nextX] == 0) {
            book[nextY][nextX] = -1;
            dfs(dfsMap, book, nextX, nextY, color);
        }
    }
}

int num = -10;
//获取最多可以走到的格子数
int GetTraversedNum(int** gameMap, int locationX, int locationY) {
    int** dfsMap = array_copy(gameMap);
    int** book = array_init(MAP_WIDTH_LENGTH, MAP_HEIGHT_LENGTH);
    //先染色,计算有几个"岛"
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            if (array_get(dfsMap, i, j) == 0) {
                num--;
                book[i][j] = -1;
                dfs(dfsMap, book, j, i, num);
            }
        }
    }
    //array_print(dfsMap);
    //将岛分类
    map<int, vector<string>> 岛;
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            if (dfsMap[i][j] <= -10) {
                vector<string> 岛坐标 = 岛[dfsMap[i][j]];
                岛坐标.push_back(std::to_string(j) + "," + std::to_string(i));
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
    array_delete(dfsMap);
    array_delete(book);
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

int stone_num = 0;
static volatile atomic_int maxPathTraversed = 0;
vector<MapInfo> mapInfoList;
set<string> 算过的路径 = {};

void GO(MapInfo& mapInfo, int startY, int startX, int deep) {
    int** oldMap = mapInfo.gameMap;
    int maxCanTraversed = GetTraversedNum(oldMap, startX, startY);
    if (MAP_WIDTH_LENGTH * MAP_HEIGHT_LENGTH - stone_num + 3 == maxPathTraversed) {
        return;
    }
    //如果当前能到的位置加上,都没有最大值大的话,直接返回
    if (maxPathTraversed >= mapInfo.path.size() + 1 + maxCanTraversed) {
        return;
    }
    string oldMapStr = array_to_string(oldMap) + std::to_string(startX) + to_string(startY);
    if (算过的路径.find(oldMapStr) != 算过的路径.end()) {
        return;
    }
    算过的路径.insert(oldMapStr);

    MapInfo newMapInfo1 = { NULL, {} }, newMapInfo2 = { NULL, {} }, newMapInfo3 = { NULL, {} }, newMapInfo4 = { NULL, {} };
    bool goNext = false;
    if (canGoNext(oldMap, startY, startX + 1)) {
        int** newMap = array_copy(oldMap);
        newMap[startY][startX + 1] = -1;
        newMapInfo1 = mapInfo;
        newMapInfo1.gameMap = newMap;
        newMapInfo1.path.push_back(to_string(startX + 1) + ", " + to_string(startY));
        GO(newMapInfo1, startY, startX + 1, deep + 1);
        array_delete(newMap);
        goNext = true;
    }
    if (canGoNext(oldMap, startY + 1, startX)) {
        int** newMap = array_copy(oldMap);
        newMap[startY + 1][startX] = -1;
        newMapInfo2 = mapInfo;
        newMapInfo2.gameMap = newMap;
        newMapInfo2.path.push_back(to_string(startX) + ", " + to_string(startY + 1));
        GO(newMapInfo2, startY + 1, startX, deep + 1);
        array_delete(newMap);
        goNext = true;
    }
    if (canGoNext(oldMap, startY, startX - 1)) {
        int** newMap = array_copy(oldMap);
        newMap[startY][startX - 1] = -1;
        newMapInfo3 = mapInfo;
        newMapInfo3.gameMap = newMap;
        newMapInfo3.path.push_back(to_string(startX - 1) + ", " + to_string(startY));
        GO(newMapInfo3, startY, startX - 1, deep + 1);
        array_delete(newMap);
        goNext = true;
    }
    if (canGoNext(oldMap, startY - 1, startX)) {
        int** newMap = array_copy(oldMap);
        newMap[startY - 1][startX] = -1;
        newMapInfo4 = mapInfo;
        newMapInfo4.gameMap = newMap;
        newMapInfo4.path.push_back(to_string(startX) + ", " + to_string(startY - 1));
        GO(newMapInfo4, startY - 1, startX, deep + 1);
        array_delete(newMap);
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
    if (vector_2_json_string(mapInfo.path) 
        == "[\"0, 5\", \"0, 4\", \"0, 3\", \"0, 2\", \"1, 2\", \"2, 2\", \"2, 1\", \"1, 1\", \"0, 1\", \"0, 0\", \"1, 0\", \"2, 0\", \"3, 0\", \"4, 0\", \"4, 1\", \"4, 2\", \"3, 2\", \"3, 3\", \"2, 3\", \"2, 4\", \"1, 4\", \"1, 5\", \"1, 6\", \"2, 6\", \"2, 5\", \"3, 5\", \"3, 4\", \"4, 4\", \"4, 3\", \"5, 3\", \"5, 2\", \"6, 2\", \"6, 3\", \"6, 4\", \"5, 4\", \"5, 5\", \"6, 5\", \"6, 6\", \"5, 6\", \"4, 6\"]") {
        cout << "123123123" << endl;
    }
    vector_delete(&mapInfo.path);
}

void one_line_main(char* map, char* resStr) {
    //将转入参数转为二维数组
    string mapStr = map;
    //std::cout << "map:" + mapStr << std::endl;
    int** mainMap = new int* [MAP_HEIGHT_LENGTH];
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        int index = mapStr.find_first_of(",");
        //获取16进制数的字符串
        string mapStrH = mapStr.substr(0, index);
        //转化为2进制数的字符串,每位就是是否有石头
        string mapStrB = hex_string_2_bin_string(mapStrH);
        mainMap[i] = new int[MAP_WIDTH_LENGTH];
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            string position = mapStrB.substr(0, 1);
            //cout << position + " ";
            if (position == "1") {
                stone_num++;
            }
            mainMap[i][j] = atoi(string_2_char(position));
            mapStrB = mapStrB.substr(1);
        }
        //cout << endl;
        //去掉开头的'xxx,'
        mapStr = mapStr.substr(index + 1);
    }
    //array_print(mainMap);
    //初始化地图
    int startX = 0, startY = 6;
    mainMap[startY][startX] = -1;
    //记录石头坐标
    vector<string> stoneLocationList;
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            if (mainMap[i][j] == 1) {
                stoneLocationList.push_back(std::to_string(j) + "," + to_string(i));
            }
        }
    }
    //排列组合,取三个
    set<int> intSet;
    vector<int**> maps;
    for (int x = 0; x < stoneLocationList.size(); x++) {
        for (int y = 0; y < stoneLocationList.size(); y++) {
            for (int z = 0; z < stoneLocationList.size(); z++) {
                if (x == y || x == z || y == z) {
                    continue;
                }
                int* arr = array_sort(new int[] {x, y, z });
                int no = arr[0] * 100 + arr[1] * 10 + arr[2];
                if (intSet.find(no) == intSet.end()) {
                    intSet.insert(no);
                    int** newMap = array_copy(mainMap);
                    string stone1 = stoneLocationList[x];
                    string stone2 = stoneLocationList[y];
                    string stone3 = stoneLocationList[z];
                    int noX = atoi(stone1.substr(0, 1).c_str());
                    int noY = atoi(stone1.substr(2, 1).c_str());
                    newMap[noY][noX] = 0;
                    noX = atoi(stone2.substr(0, 1).c_str());
                    noY = atoi(stone2.substr(2, 1).c_str());
                    newMap[noY][noX] = 0;
                    noX = atoi(stone3.substr(0, 1).c_str());
                    noY = atoi(stone3.substr(2, 1).c_str());
                    newMap[noY][noX] = 0;
                    maps.push_back(newMap);
                }
            }
        }
    }
    //cout << "数量:" + std::to_string(maps.size()) << endl;
    for (int i = 0; i < maps.size(); i++) {
        MapInfo mapInfo = { maps[i], vector<string>() };
        GO(mapInfo, startY, startX, 0);
        array_delete(mapInfo.gameMap);
        vector_delete(&mapInfo.path);
        //string outstr = "完成:" + std::to_string(i) + ", 最大面积:" + std::to_string(mapInfoList[mapInfoList.size() - 1].path.size());
        //cout << outstr << endl;
    }
    set_delete(&intSet);
    //std::cout << "算过的路径:" + std::to_string(算过的路径.size()) << std::endl;
    std::cout << "maxPathTraversed:" + std::to_string(mapInfoList[mapInfoList.size() - 1].path.size()) << std::endl;
    vector_print(mapInfoList[mapInfoList.size() - 1].path);

    std::map<string, string> resMap;
    resMap["path"] = vector_2_json_string(mapInfoList[mapInfoList.size() - 1].path);
    resMap["calPathNum"] = to_string(算过的路径.size());
    set_delete(&算过的路径);
    //std::cout << "res map:" + map_2_json_string(resMap) << std::endl;
    strcpy(resStr, string_2_char(map_2_json_string(resMap)));
}
