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

//Ⱦɫ
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
//��ȡ�������ߵ��ĸ�����
int GetTraversedNum(int** gameMap, int locationX, int locationY) {
    int** dfsMap = array_copy(gameMap);
    int** book = array_init(MAP_WIDTH_LENGTH, MAP_HEIGHT_LENGTH);
    //��Ⱦɫ,�����м���"��"
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
    //��������
    map<int, vector<string>> ��;
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            if (dfsMap[i][j] <= -10) {
                vector<string> ������ = ��[dfsMap[i][j]];
                ������.push_back(std::to_string(j) + "," + std::to_string(i));
                ��[dfsMap[i][j]] = ������;
            }
        }
    }
    //print_map(��);
    //��ȡ��һ�����ߵ��ĵ�
    int maxCanTraversed = 0;
    for (map<int, vector<string>>::iterator iter = ��.begin(); iter != ��.end(); iter++) {
        int islandNum = iter->first;
        vector<string> ������ = iter->second;
        bool canGo = false;
        for (int k = 0; k < 4; k++) {
            int nextX = locationX + nextArr[k][0];
            int nextY = locationY + nextArr[k][1];
            if (std::find(������.begin(), ������.end(), to_string(nextX) + "," + to_string(nextY)) != ������.end()) {
                canGo = true;
                break;
            }
        }
        if (canGo) {
            //�������ߵ��ĵ������ȷ��������ߵ���������
            int canTraversed = ������.size();
            if (������.size() > maxCanTraversed) {
                maxCanTraversed = ������.size();
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

static volatile atomic_int maxPathTraversed = 0;
vector<MapInfo> mapInfoList;
set<string> �����·�� = {};

void GO(MapInfo& mapInfo, int startY, int startX, int deep) {
    int** oldMap = mapInfo.gameMap;
    int maxCanTraversed = GetTraversedNum(oldMap, startX, startY);
    if (MAP_WIDTH_LENGTH * MAP_HEIGHT_LENGTH - 3 == maxPathTraversed) {
        return;
    }
    //�����ǰ�ܵ���λ�ü���,��û�����ֵ��Ļ�,ֱ�ӷ���
    if (maxPathTraversed >= mapInfo.path.size() + 1 + maxCanTraversed) {
        return;
    }
    string oldMapStr = array_to_string(oldMap) + std::to_string(startX) + to_string(startY);
    if (�����·��.find(oldMapStr) != �����·��.end()) {
        return;
    }
    �����·��.insert(oldMapStr);

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
    vector_delete(&mapInfo.path);
}

void one_line_main(char* map, char* resStr) {
    //��ת�����תΪ��ά����
    string mapStr = map;
    //std::cout << "map:" + mapStr << std::endl;
    int** mainMap = new int* [MAP_HEIGHT_LENGTH];
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        int index = mapStr.find_first_of(",");
        //��ȡ16���������ַ���
        string mapStrH = mapStr.substr(0, index);
        //ת��Ϊ2���������ַ���,ÿλ�����Ƿ���ʯͷ
        string mapStrB = hex_string_2_bin_string(mapStrH);
        mainMap[i] = new int[MAP_WIDTH_LENGTH];
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            string position = mapStrB.substr(0, 1);
            //cout << position + " ";
            mainMap[i][j] = atoi(string_2_char(position));
            mapStrB = mapStrB.substr(1);
        }
        //cout << endl;
        //ȥ����ͷ��'xxx,'
        mapStr = mapStr.substr(index + 1);
    }
    //array_print(mainMap);
    //��ʼ����ͼ
    int startX = 0, startY = 6;
    mainMap[startY][startX] = -1;
    //��¼ʯͷ����
    vector<string> stoneLocationList;
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            if (mainMap[i][j] == 1) {
                stoneLocationList.push_back(std::to_string(j) + "," + to_string(i));
            }
        }
    }
    //�������,ȡ����
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
    //cout << "����:" + std::to_string(maps.size()) << endl;
    for (int i = 0; i < maps.size(); i++) {
        MapInfo mapInfo = { maps[i], vector<string>() };
        GO(mapInfo, startY, startX, 0);
        array_delete(mapInfo.gameMap);
        vector_delete(&mapInfo.path);
        //string outstr = "���:" + std::to_string(i) + ", ������:" + std::to_string(mapInfoList[mapInfoList.size() - 1].path.size());
        //cout << outstr << endl;
    }
    set_delete(&intSet);
    //std::cout << "�����·��:" + std::to_string(�����·��.size()) << std::endl;
    //std::cout << "maxPathTraversed:" + std::to_string(mapInfoList[mapInfoList.size() - 1].path.size()) << std::endl;
    //vector_print(mapInfoList[mapInfoList.size() - 1].path);

    std::map<string, string> resMap;
    resMap["path"] = vector_2_json_string(mapInfoList[mapInfoList.size() - 1].path);
    resMap["calPathNum"] = to_string(�����·��.size());
    set_delete(&�����·��);
    //std::cout << "res map:" + map_2_json_string(resMap) << std::endl;
    strcpy(resStr, string_2_char(map_2_json_string(resMap)));
}
