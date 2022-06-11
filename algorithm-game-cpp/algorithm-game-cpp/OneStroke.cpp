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
 * ���������Ϊ2,��Ϊ3
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

//Ⱦɫ
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
//��ȡ�������ߵ��ĸ�����
int GetTraversedNum(int** gameMap, int locationX, int locationY) {
    int** dfsMap = copy_array(gameMap);
    int** book = init_array(MAP_WIDTH_LENGTH, MAP_HEIGHT_LENGTH);
    //��Ⱦɫ,�����м���"��"
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
    //��������
    map<int, vector<string>> ��;
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            if (dfsMap[i][j] <= -10) {
                vector<string> ������ = ��[dfsMap[i][j]];
                ������.push_back(to_string(j) + "," + to_string(i));
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

void GO(MapInfo &mapInfo, int startY, int startX, int deep) {
    int** oldMap = mapInfo.gameMap;
    int maxCanTraversed = GetTraversedNum(oldMap, startX, startY);
    if (MAP_WIDTH_LENGTH * MAP_HEIGHT_LENGTH - 3 == maxPathTraversed) {
        return;
    }
    //�����ǰ�ܵ���λ�ü���,��û�����ֵ��Ļ�,ֱ�ӷ���
    if (maxPathTraversed >= mapInfo.path.size() + 1 + maxCanTraversed) {
        return;
    }
    string oldMapStr = get_string_array(oldMap) + to_string(startX) + to_string(startY);
    if (�����·��.find(oldMapStr) != �����·��.end()) {
        return;
    }
    �����·��.insert(oldMapStr);

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

    std::cout << "�����·��:" + to_string(�����·��.size()) << std::endl;
    std::cout << "maxPathTraversed:" + to_string(maxPathTraversed) << std::endl;
    print_vector(mapInfoList[mapInfoList.size() - 1].path);
}

