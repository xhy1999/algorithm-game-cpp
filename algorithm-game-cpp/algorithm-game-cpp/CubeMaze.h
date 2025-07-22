#pragma once
#include <iostream>
#include <vector>
#include "VectorUtils.h"

using namespace std;

//空
const int EMPTY = 9;
//目标数字,中心3x3区域需变成的值
const int TARGET = 1;
//非目标
const int OTHER = 0;

//(0[上]1[右]2[下]3[左])
const int OP_UP = 0;
const int OP_RIGHT = 1;
const int OP_DOWN = 2;
const int OP_LEFT = 3;

//(操作行/列的索引,0-2,左到右,上到下)
const int OP_INDEX_3 = 0;
const int OP_INDEX_4 = 1;
const int OP_INDEX_5 = 2;

//地图大小为9x9
const int MAP_SIZE = 9;
//地图开始:0
const int MAP_START = 0;
//地图结束:MAP_SIZE - 1 = 8
const int MAP_END = MAP_SIZE - 1;
//地图开始索引:3
const int MAP_INDEX_START = 3;
//地图结束索引:5
const int MAP_INDEX_END = 5;
//最大搜索步数
const int MAX_DEPTH = 15;

struct CubeMap {

    //当前地图
    int map[MAP_SIZE][MAP_SIZE];
    //当前步数(从起点到此状态)
    int step;
    //启发值(中心区域与目标的偏差程度)
    int h;
    //到达此状态的路径记录,格式为:数字(操作行/列的索引)+数字(0[上]1[右]2[下]3[左])
    //一个16进制数字 高2位(操作行/列的索引,0-2,左到右,上到下) 低2位(0[上]1[右]2[下]3[左])
    std::vector<int> opPath;

    CubeMap(const int src[9][9], int step_, int h_, const std::vector<int>& path) {
        this->step = step_;
        this->h = h_;
        this->opPath = path;
        // 拷贝地图
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                this->map[i][j] = src[i][j];
    }

    void release() {
        std::vector<int>().swap(opPath);
    }

    //A* 算法中的 f(n) = g(n) + h(n)
    int f() const { return step + h; }

    //获取地图中所有目标数量
    int targetNum() const {
        int num = 0;
        for (int i = MAP_START; i < MAP_SIZE; i++) {
            for (int j = MAP_START; j < MAP_SIZE; j++) {
                if (map[i][j] == TARGET) {
                    num++;
                }
            }
        }
        return num;
    }

    //运算符重载：priority_queue 默认是"大顶堆"
    //为实现"小顶堆",我们让f小的优先,需重载 >
    bool operator>(const CubeMap& other) const {
        return this->f() > other.f();
    }

};

void cube_maze_main();