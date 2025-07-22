#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <chrono>
#include "CubeMaze.h"
#include "ArrayUtils.h"
#include "VectorUtils.h"

using namespace std;

//节点扩展计数器
static int nodesExpanded = 0;
//记录搜索开始时间
chrono::steady_clock::time_point startTime;


//压缩地图 EMPTY会被压缩为OTHER
static uint64_t compress(const int map[MAP_SIZE][MAP_SIZE]) {
    uint64_t val = 0L;
    // 第0-2行中心3列
    for (int i = MAP_START; i < MAP_INDEX_START; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            val = (val << 1) | ((map[i][j] == EMPTY ? 0 : map[i][j]) & 1);
        }
    }
    // 第3-5行整行9列
    for (int i = MAP_INDEX_START; i <= MAP_INDEX_END; i++) {
        for (int j = MAP_START; j <= MAP_END; j++) {
            val = (val << 1) | ((map[i][j] == EMPTY ? 0 : map[i][j]) & 1);
        }
    }
    // 第6-8行中心3列
    for (int i = MAP_INDEX_END + 1; i < MAP_SIZE; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            val = (val << 1) | ((map[i][j] == EMPTY ? 0 : map[i][j]) & 1);
        }
    }
    return val;
}


//解压地图 不区分EMPTY
static void deCompress(uint64_t val, int map[MAP_SIZE][MAP_SIZE]) {
    //初始化数组为 EMPTY
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            map[i][j] = EMPTY;
        }
    }
    //第6-8行中心3列（共9位）
    for (int i = MAP_SIZE - 1; i >= MAP_INDEX_END + 1; --i) {
        for (int j = MAP_INDEX_END; j >= MAP_INDEX_START; --j) {
            map[i][j] = (int)(val & 1);
            val >>= 1;
        }
    }
    //第3-5行全部9列（共27位）
    for (int i = MAP_INDEX_END; i >= MAP_INDEX_START; --i) {
        for (int j = MAP_END; j >= MAP_START; --j) {
            map[i][j] = (int)(val & 1);
            val >>= 1;
        }
    }
    //第0-2行中心3列（共9位）
    for (int i = MAP_INDEX_START - 1; i >= MAP_START; --i) {
        for (int j = MAP_INDEX_END; j >= MAP_INDEX_START; --j) {
            map[i][j] = (int)(val & 1);
            val >>= 1;
        }
    }
}

/************** 数组相关方法 **************/

static void array_copy(const int src[MAP_SIZE][MAP_SIZE], int target[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            target[i][j] = src[i][j];
}

static void array_print(const int arr[MAP_SIZE][MAP_SIZE]) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            std::cout << arr[i][j];
        }
        std::cout << '\n';
    }
    std::cout << "COMP:" << compress(arr);
    std::cout << '\n';
}

/************** 操作相关方法 **************/

//(操作行/列的索引,0-2,左到右,上到下) 最终索引要 +3
static int getOpIndex(int op) {
    //提取高2位:右移2位后取低2位
    return ((op >> 2) & 0b11);
}

//(0[上]1[右]2[下]3[左])
static int getOp(int op) {
    //提取低2位:直接取低2位
    return op & 0b11;
}

/************** 求解相关方法 **************/

//启发函数:中心区域中不等于目标的格子数
static int heuristic(const int map[MAP_SIZE][MAP_SIZE]) {
    int mismatch = 0;
    for (int i = MAP_INDEX_START; i <= MAP_INDEX_END; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            if (map[i][j] != TARGET) {
                mismatch += 1;
            }
        }
    }
    return mismatch;
}

//剪枝:判断当前操作是否是上一步的反操作
static bool isReverse(const std::vector<int> path, int move) {
    if (path.empty()) {
        return false;
    }
    int last = path.back();
    //操作的索引一样,同时为逆操作14/23
    return ((getOpIndex(last) == getOpIndex(move))) && (getOp(last) + getOp(move) == 3);
}

//选定行左移 row为索引
static void moveRowLeft(const int map[MAP_SIZE][MAP_SIZE], int row, int res[MAP_SIZE][MAP_SIZE]) {
    array_copy(map, res);
    for (int j = MAP_START; j < MAP_END; j++) {
        res[row][j] = map[row][j + 1];
    }
    res[row][MAP_END] = EMPTY;
}

//判断选定行能否左移
static bool canRowLeft(int map[MAP_SIZE][MAP_SIZE], int row) {
    return map[row][MAP_INDEX_END + 1] != EMPTY;
}

//选定行右移 row为索引
static void moveRowRight(const int map[MAP_SIZE][MAP_SIZE], int row, int res[MAP_SIZE][MAP_SIZE]) {
    array_copy(map, res);
    for (int j = MAP_END; j > MAP_START; j--) {
        res[row][j] = map[row][j - 1];
    }
    res[row][MAP_START] = EMPTY;
}

//判断选定行能否右移
static bool canRowRight(int map[MAP_SIZE][MAP_SIZE], int row) {
    return map[row][MAP_INDEX_START - 1] != EMPTY;
}

//选定列上移 col为索引
static void moveColUp(const int map[MAP_SIZE][MAP_SIZE], int col, int res[MAP_SIZE][MAP_SIZE]) {
    array_copy(map, res);
    for (int i = MAP_START; i < MAP_END; i++) {
        res[i][col] = map[i + 1][col];
    }
    res[MAP_END][col] = EMPTY;
}

//判断选定列能否上移
static bool canColUp(const int map[MAP_SIZE][MAP_SIZE], int col) {
    return map[MAP_INDEX_END + 1][col] != EMPTY;
}

//选定列下移
static void moveColDown(const int map[MAP_SIZE][MAP_SIZE], int col, int res[MAP_SIZE][MAP_SIZE]) {
    array_copy(map, res);
    for (int i = MAP_END; i > MAP_START; i--) {
        res[i][col] = map[i - 1][col];
    }
    res[MAP_START][col] = EMPTY;
}

//判断选定列能否下移
static bool canColDown(int map[MAP_SIZE][MAP_SIZE], int col) {
    return map[MAP_INDEX_START - 1][col] != EMPTY;
}

//优先队列,按f值排序
std::priority_queue<CubeMap, std::vector<CubeMap>, std::greater<CubeMap>> pq;
//用于存储已访问状态的压缩序列化字符串
std::unordered_set<uint64_t> visited;

//A* 搜索主逻辑
static std::vector<int> solve(int startGrid[MAP_SIZE][MAP_SIZE]) {
    visited.insert(compress(startGrid));
    // 初始启发值
    int h0 = heuristic(startGrid);
    pq.push(CubeMap(startGrid, 0, h0, {}));
    //重置计数器
    nodesExpanded = 0;
    //记录开始时间
    startTime = chrono::steady_clock::now();
    while (!pq.empty()) {
        //取出当前f值最小的状态
        CubeMap curr = pq.top();
        pq.pop();
        if (nodesExpanded++ % 100000 == 0) {
            auto now = chrono::steady_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
            cout << "已计算" << nodesExpanded << "条路径,耗时:" << duration << "ms" << endl;
        }
        //如果已达目标状态，返回路径
        if (curr.h == 0) {
            return curr.opPath;
        }
        //步数下限剪枝
        /*if (curr.step + curr.h > MAX_DEPTH) {
            curr.release();
            continue;
        }*/
        //超过最大步数
        /*if (curr.step >= MAX_DEPTH) {
            curr.release();
            continue;
        }*/
        //判断总目标是否足够
        if (curr.targetNum() < 9) {
            continue;
        }
        //尝试对第3~5行进行左右移动
        for (int i = MAP_INDEX_START; i <= MAP_INDEX_END; i++) {
            //左移
            if (canRowLeft(curr.map, i)) {
                //生成操作
                int op = ((i - 3) << 2) | OP_LEFT;
                //生成左移后的地图
                int leftMap[MAP_SIZE][MAP_SIZE];
                moveRowLeft(curr.map, i, leftMap);
                uint64_t keyLeft = compress(leftMap);
                //没有计算左移后的地图,且不是上一步的逆操作
                if (visited.insert(keyLeft).second && !isReverse(curr.opPath, op)) {
                    std::vector<int> newPath = curr.opPath;
                    newPath.push_back(op);
                    pq.push(CubeMap(leftMap, curr.step + 1, heuristic(leftMap), newPath));
                }
            }
            //右移
            if (canRowRight(curr.map, i)) {
                int op = ((i - 3) << 2) | OP_RIGHT;
                int rightMap[MAP_SIZE][MAP_SIZE];
                moveRowRight(curr.map, i, rightMap);
                uint64_t keyRight = compress(rightMap);
                if (visited.insert(keyRight).second && !isReverse(curr.opPath, op)) {
                    std::vector<int> newPath = curr.opPath;
                    newPath.push_back(op);
                    pq.push(CubeMap(rightMap, curr.step + 1, heuristic(rightMap), newPath));
                }
            }
        }
        //尝试对第3~5列进行上下移动
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            //上移
            if (canColUp(curr.map, j)) {
                int op = ((j - 3) << 2) | OP_UP;
                int upMap[MAP_SIZE][MAP_SIZE];
                moveColUp(curr.map, j, upMap);
                uint64_t keyUp = compress(upMap);
                if (visited.insert(keyUp).second && !isReverse(curr.opPath, op)) {
                    std::vector<int> newPath = curr.opPath;
                    newPath.push_back(op);
                    pq.push(CubeMap(upMap, curr.step + 1, heuristic(upMap), newPath));
                }
            }
            //下移
            if (canColDown(curr.map, j)) {
                int op = ((j - 3) << 2) | OP_DOWN;
                int downMap[MAP_SIZE][MAP_SIZE];
                moveColDown(curr.map, j, downMap);
                uint64_t keyDown = compress(downMap);
                if (visited.insert(keyDown).second && !isReverse(curr.opPath, op)) {
                    std::vector<int> newPath = curr.opPath;
                    newPath.push_back(op);
                    pq.push(CubeMap(downMap, curr.step + 1, heuristic(downMap), newPath));
                }
            }
        }
    }
    //未找到解
    return {};
}

void cube_maze_main() {
    /*int map[MAP_SIZE][MAP_SIZE] = {
                {9, 9, 9, 0, 0, 0, 9, 9, 9},
                {9, 9, 9, 0, 0, 0, 9, 9, 9},
                {9, 9, 9, 0, 1, 0, 9, 9, 9},
                {0, 0, 0, 0, 0, 1, 0, 1, 0},
                {1, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 1},
                {9, 9, 9, 0, 0, 0, 9, 9, 9},
                {9, 9, 9, 0, 0, 0, 9, 9, 9},
                {9, 9, 9, 1, 1, 1, 9, 9, 9}
    };*/
    int map[MAP_SIZE][MAP_SIZE];
    deCompress(28558319943680, map);
    // 调用solve函数寻找解路径
    auto startTime = chrono::steady_clock::now();
    vector<int> result = solve(map);

    std::priority_queue<CubeMap, std::vector<CubeMap>, std::greater<CubeMap>>().swap(pq);
    std::unordered_set<uint64_t>().swap(visited);

    auto endTime = chrono::steady_clock::now();
    if (!result.empty()) {
        cout << "找到解决方案，总共移动步数：" << result.size() << "\n";
        cout << "移动路径如下：" << endl;
        for (int x : result) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
        int currentMap[MAP_SIZE][MAP_SIZE];
        // 复制map到currentMap，假设有copy函数
        for (int i = 0; i < MAP_SIZE; i++)
            for (int j = 0; j < MAP_SIZE; j++)
                currentMap[i][j] = map[i][j];
        for (int op : result) {
            cout << "→ " << op << endl;
            int opIndex = 3 + getOpIndex(op);
            switch (getOp(op)) {
                case OP_UP:
                    cout << "上移第" << (opIndex + 1) << "列" << endl;
                    moveColUp(currentMap, opIndex, currentMap);
                    break;
                case OP_RIGHT:
                    cout << "右移第" << (opIndex + 1) << "行" << endl;
                    moveRowRight(currentMap, opIndex, currentMap);
                    break;
                case OP_DOWN:
                    cout << "下移第" << (opIndex + 1) << "列" << endl;
                    moveColDown(currentMap, opIndex, currentMap);
                    break;
                case OP_LEFT:
                    cout << "左移第" << (opIndex + 1) << "行" << endl;
                    moveRowLeft(currentMap, opIndex, currentMap);
                    break;
                default:
                    break;
            }
            array_print(currentMap);
            cout << endl;
        }
    }
    else {
        cout << "未能在" << MAX_DEPTH << "步内找到解。" << endl;
    }

    cout << "扩展节点数: " << nodesExpanded << endl;
    cout << "总耗时: "
        << chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count()
        << " 毫秒" << endl;

}