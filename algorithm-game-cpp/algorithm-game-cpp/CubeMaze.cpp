#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <chrono>
#include <algorithm>
#include <random>
#include "CubeMaze.h"
#include "ArrayUtils.h"
#include "VectorUtils.h"

using namespace std;

//节点扩展计数器
static int nodesExpanded = 0;
//记录搜索开始时间
chrono::steady_clock::time_point startTime;


//压缩地图 EMPTY会被压缩为OTHER
static uint64_t compress(int** map) {
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

int map_get(int** arr, int var0, int var1) {
    return *(arr[var0] + var1);
}

void map_set(int** arr, int var0, int var1, int value) {
    arr[var0][var1] = value;
}

void map_print(int** arr) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            std::cout << std::to_string(map_get(arr, i, j));
        }
        std::cout << std::endl;
    }
    std::cout << "COMP:" << compress(arr);
    std::cout << '\n';
}

//深拷贝地图
int** map_copy(int** map) {
    int** mainMap = new int* [MAP_SIZE];
    for (int i = 0; i < MAP_SIZE; i++) {
        mainMap[i] = new int[MAP_SIZE];
        for (int j = 0; j < MAP_SIZE; j++) {
            mainMap[i][j] = map_get(map, i, j);
        }
    }
    return mainMap;
}

//释放地图内存
void map_delete(int** arr) {
    for (int i = 0; i < MAP_SIZE; ++i) {
        delete[] arr[i];
        arr[i] = NULL;
    }
    delete[] arr;
    arr = NULL;
}

//初始化地图
int** map_init(int initVal) {
    int** arr = new int* [MAP_SIZE];
    for (int i = 0; i < MAP_SIZE; i++) {
        arr[i] = new int[MAP_SIZE];
        for (int j = 0; j < MAP_SIZE; j++) {
            arr[i][j] = initVal;
        }
    }
    return arr;
}

/************** 数组相关方法 **************/

//static void array_copy(const int src[MAP_SIZE][MAP_SIZE], int target[MAP_SIZE][MAP_SIZE]) {
//    for (int i = 0; i < MAP_SIZE; ++i)
//        for (int j = 0; j < MAP_SIZE; ++j)
//            target[i][j] = src[i][j];
//}
//
//static void array_print(const int arr[MAP_SIZE][MAP_SIZE]) {
//    for (int i = 0; i < MAP_SIZE; i++) {
//        for (int j = 0; j < MAP_SIZE; j++) {
//            std::cout << arr[i][j];
//        }
//        std::cout << '\n';
//    }
//    std::cout << "COMP:" << compress(arr);
//    std::cout << '\n';
//}

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
static int heuristic(int** map) {
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
static int** moveRowLeft(int** map, int row) {
    int** res = map_copy(map);
    for (int j = MAP_START; j < MAP_END; j++) {
        res[row][j] = map[row][j + 1];
    }
    res[row][MAP_END] = EMPTY;
    return res;
}

//判断选定行能否左移
static bool canRowLeft(int** map, int row) {
    return map[row][MAP_INDEX_END + 1] != EMPTY;
}

//选定行右移 row为索引
static int** moveRowRight(int** map, int row) {
    int** res = map_copy(map);
    for (int j = MAP_END; j > MAP_START; j--) {
        res[row][j] = map[row][j - 1];
    }
    res[row][MAP_START] = EMPTY;
    return res;
}

//判断选定行能否右移
static bool canRowRight(int** map, int row) {
    return map[row][MAP_INDEX_START - 1] != EMPTY;
}

//选定列上移 col为索引
static int** moveColUp(int** map, int col) {
    int** res = map_copy(map);
    for (int i = MAP_START; i < MAP_END; i++) {
        res[i][col] = map[i + 1][col];
    }
    res[MAP_END][col] = EMPTY;
    return res;
}

//判断选定列能否上移
static bool canColUp(int** map, int col) {
    return map[MAP_INDEX_END + 1][col] != EMPTY;
}

//选定列下移
static int** moveColDown(int** map, int col) {
    int** res = map_copy(map);
    for (int i = MAP_END; i > MAP_START; i--) {
        res[i][col] = map[i - 1][col];
    }
    res[MAP_START][col] = EMPTY;
    return res;
}

//判断选定列能否下移
static bool canColDown(int** map, int col) {
    return map[MAP_INDEX_START - 1][col] != EMPTY;
}

//优先队列,按f值排序
std::priority_queue<CubeMap, std::vector<CubeMap>, std::greater<CubeMap>> pq;
//用于存储已访问状态的压缩序列化字符串
std::unordered_set<uint64_t> visited;

//A* 搜索主逻辑
static std::vector<int> solve(int** startGrid) {
    visited.insert(compress(startGrid));
    // 初始启发值
    int h0 = heuristic(startGrid);
    pq.push(CubeMap(startGrid, 0, h0, {}, NULL));
    //重置计数器
    nodesExpanded = 0;
    //记录开始时间
    startTime = chrono::steady_clock::now();
    while (!pq.empty()) {
        //取出当前f值最小的状态
        CubeMap curr = pq.top();
        pq.pop();
        //计算过多剪枝 防止内存问题
        if (nodesExpanded >= 800000) {
            return {};
        }
        if (nodesExpanded++ % 100000 == 0) {
            auto now = chrono::steady_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
            cout << "已计算" << nodesExpanded << "条路径,耗时:" << duration << "ms" << endl;
        }
        //如果已达目标状态，返回路径
        if (curr.h == 0) {
            //curr.release();
            //vector_delete(&curr.opPath);
            //map_delete(curr.gameMap);
            return curr.opPath;
        }
        //步数下限剪枝
        if (curr.step + curr.h > MAX_DEPTH) {
            //curr.release();
            //vector_delete(&curr.opPath);
            //map_delete(curr.gameMap);
            continue;
        }
        //超过最大步数
        if (curr.step >= MAX_DEPTH) {
            //curr.release();
            //vector_delete(&curr.opPath);
            //map_delete(curr.gameMap);
            continue;
        }
        //判断总目标是否足够
        if (curr.targetNum() < 9) {
            //curr.release();
            //vector_delete(&curr.opPath);
            //map_delete(curr.gameMap);
            continue;
        }
        //尝试对第3~5行进行左右移动
        for (int i = MAP_INDEX_START; i <= MAP_INDEX_END; i++) {
            //左移
            if (canRowLeft(curr.gameMap, i)) {
                //生成操作
                int op = ((i - 3) << 2) | OP_LEFT;
                //生成左移后的地图
                int** leftMap = moveRowLeft(curr.gameMap, i);
                uint64_t keyLeft = compress(leftMap);
                //没有计算左移后的地图,且不是上一步的逆操作
                if (visited.insert(keyLeft).second && !isReverse(curr.opPath, op)) {
                    pq.push(CubeMap(leftMap, curr.step + 1, heuristic(leftMap), curr.opPath, op));
                }
            }
            //右移
            if (canRowRight(curr.gameMap, i)) {
                int op = ((i - 3) << 2) | OP_RIGHT;
                int** rightMap = moveRowRight(curr.gameMap, i);
                uint64_t keyRight = compress(rightMap);
                if (visited.insert(keyRight).second && !isReverse(curr.opPath, op)) {
                    pq.push(CubeMap(rightMap, curr.step + 1, heuristic(rightMap), curr.opPath, op));
                }
            }
        }
        //尝试对第3~5列进行上下移动
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            //上移
            if (canColUp(curr.gameMap, j)) {
                int op = ((j - 3) << 2) | OP_UP;
                int** upMap = moveColUp(curr.gameMap, j);
                uint64_t keyUp = compress(upMap);
                if (visited.insert(keyUp).second && !isReverse(curr.opPath, op)) {
                    pq.push(CubeMap(upMap, curr.step + 1, heuristic(upMap), curr.opPath, op));
                }
            }
            //下移
            if (canColDown(curr.gameMap, j)) {
                int op = ((j - 3) << 2) | OP_DOWN;
                int** downMap = moveColDown(curr.gameMap, j);
                uint64_t keyDown = compress(downMap);
                if (visited.insert(keyDown).second && !isReverse(curr.opPath, op)) {
                    pq.push(CubeMap(downMap, curr.step + 1, heuristic(downMap), curr.opPath, op));
                }
            }
        }
        //curr.release();
        //vector_delete(&curr.opPath);
        //map_delete(curr.gameMap);
    }
    //未找到解
    return {};
}

int** random_map() {
    int** map = map_init(EMPTY);
    std::vector<int> nums;
    // 将 2~6 每个数字加入 9 次
    for (int i = 2; i <= 6; i++) {
        for (int j = 0; j <= 8; j++) {
            nums.push_back(i);
        }
    }
    // Fisher-Yates 洗牌算法
    std::mt19937 rng(std::time(0)); // 使用Mersenne Twister随机数引擎
    for (int i = nums.size() - 1; i > 0; i--) {
        std::uniform_int_distribution<int> dist(0, i);
        int j = dist(rng); // 随机生成 0 到 i 的索引
        std::swap(nums[j], nums[i]);
    }
    int num = 0;
    // 第0-2行中心3列
    for (int i = MAP_START; i < MAP_INDEX_START; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            map[i][j] = nums[num++];
        }
    }
    // 第3-5行整行9列
    for (int i = MAP_INDEX_START; i <= MAP_INDEX_END; i++) {
        for (int j = MAP_START; j <= MAP_END; j++) {
            map[i][j] = nums[num++];
        }
    }
    // 第6-8行中心3列
    for (int i = MAP_INDEX_END + 1; i < MAP_SIZE; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            map[i][j] = nums[num++];
        }
    }
    return map;
}

int** convert_map(int** src, int target) {
    int** targetMap = map_init(EMPTY);
    for (int i = MAP_START; i < MAP_SIZE; i++) {
        for (int j = MAP_START; j < MAP_SIZE; j++) {
            if (src[i][j] == target) {
                targetMap[i][j] = 1;
            } else {
                targetMap[i][j] = 0;
            }
        }
    }
    return targetMap;
}

void cube_maze_main() {
    //int map[MAP_SIZE][MAP_SIZE];
    /*map[MAP_SIZE][MAP_SIZE] = {
                {9, 9, 9, 0, 0, 0, 9, 9, 9},
                {9, 9, 9, 0, 0, 0, 9, 9, 9},
                {9, 9, 9, 0, 1, 0, 9, 9, 9},
                {0, 0, 0, 0, 0, 1, 0, 1, 0},
                {1, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 1},
                {9, 9, 9, 0, 0, 0, 9, 9, 9},
                {9, 9, 9, 0, 0, 0, 9, 9, 9},
                {9, 9, 9, 1, 1, 1, 9, 9, 9}
    }; */

    int** map = random_map();
    map_print(map);
    //deCompress(28558319943680, map);
    int calPathNum = 0;
    vector<int> resList;
    bool optimal = false;
    auto startTime = chrono::steady_clock::now();
    for (int i = 2; i <= 6; i++) {
        std::priority_queue<CubeMap, std::vector<CubeMap>, std::greater<CubeMap>>().swap(pq);
        //std::vector<CubeMap> baseVec;
        //baseVec.reserve(100000);
        //std::priority_queue<CubeMap, std::vector<CubeMap>, std::greater<CubeMap>> pq(
        //    std::greater<CubeMap>(), std::move(baseVec));
        std::unordered_set<uint64_t>().swap(visited);
        visited.reserve(1 << 21);// 等价于 reserve(1048576)
        int** calcMap = convert_map(map, i);
        map_print(calcMap);
        //调用solve函数寻找解路径
        vector<int> result = solve(calcMap);
        calPathNum += nodesExpanded;
        cout << "计算路径:" << nodesExpanded << endl;
        cout << "pq:" << pq.size() << endl;
        cout << "visited:" << visited.size() << endl;
        if (!result.empty()) {
            //有解
            if (resList.empty()) {
                resList = result;
            }
            if (!resList.empty() && !optimal) {
                break;
            }
            if (resList.size() < resList.size()) {
                resList = result;
            }
        }
    }
    std::priority_queue<CubeMap, std::vector<CubeMap>, std::greater<CubeMap>>().swap(pq);
    std::unordered_set<uint64_t>().swap(visited);
    auto endTime = chrono::steady_clock::now();
    if (!resList.empty()) {
        cout << "找到解决方案，总共移动步数：" << resList.size() << "\n";
        cout << "移动路径如下：" << endl;
        for (int x : resList) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
        int** currentMap = map_copy(map);
        for (int op : resList) {
            cout << "→ " << op << endl;
            int opIndex = 3 + getOpIndex(op);
            switch (getOp(op)) {
                case OP_UP:
                    cout << "上移第" << (opIndex + 1) << "列" << endl;
                    currentMap = moveColUp(currentMap, opIndex);
                    break;
                case OP_RIGHT:
                    cout << "右移第" << (opIndex + 1) << "行" << endl;
                    currentMap = moveRowRight(currentMap, opIndex);
                    break;
                case OP_DOWN:
                    cout << "下移第" << (opIndex + 1) << "列" << endl;
                    currentMap = moveColDown(currentMap, opIndex);
                    break;
                case OP_LEFT:
                    cout << "左移第" << (opIndex + 1) << "行" << endl;
                    currentMap = moveRowLeft(currentMap, opIndex);
                    break;
                default:
                    break;
            }
            map_print(currentMap);
            cout << endl;
        }
    }
    else {
        cout << "未能在" << MAX_DEPTH << "步内找到解。" << endl;
    }

    cout << "扩展节点数: " << calPathNum << endl;
    cout << "总耗时: "
        << chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count()
        << " 毫秒" << endl;

}