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
#include "VectorUtils.h"
#include "bloom_filter.hpp"
#include <bitset>
#include <unordered_map>

using namespace std;


/************** 地图相关方法 **************/

inline int map_get(int** map, int i, int j) {
    return map[i][j];
    //return *(map[i] + j);
}

inline void map_set(int** map, int i, int j, int value) {
    map[i][j] = value;
}

void map_print(int** map) {
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            std::cout << std::to_string(map_get(map, i, j));
        }
        std::cout << std::endl;
    }
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

/************** 地图压缩相关方法 **************/

//压缩地图 EMPTY会被压缩为OTHER 需要还原的话用这个方法压缩
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

//获取四个方向EMPTY数量(4x3x3)
static int count_empty(int** map, int rowStart, int colStart, int rowEnd, int colEnd) {
    int count = 0;
    for (int i = rowStart; i < rowEnd; i++) {
        for (int j = colStart; j < colEnd; j++) {
            if (map_get(map, i, j) == EMPTY) {
                count++;
            }
        }
    }
    return count;
}

//计算时使用的压缩
static uint64_t compress(int** map) {
    uint64_t val = 0;
    //上3行中心3列
    for (int i = MAP_START; i < MAP_INDEX_START; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            int cell = map_get(map, i, j);
            val = (val << 1) | ((cell == EMPTY ? 0 : cell) & 1);
        }
    }
    //中间3行全9列
    for (int i = MAP_INDEX_START; i <= MAP_INDEX_END; i++) {
        for (int j = MAP_START; j <= MAP_END; j++) {
            int cell = map_get(map, i, j);
            val = (val << 1) | ((cell == EMPTY ? 0 : cell) & 1);
        }
    }
    //下3行中心3列
    for (int i = MAP_INDEX_END + 1; i < MAP_SIZE; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            int cell = map_get(map, i, j);
            val = (val << 1) | ((cell == EMPTY ? 0 : cell) & 1);
        }
    }
    //四个方向EMPTY数量（4x3x3）
    int topEmpty = count_empty(map, 0, 3, 3, 6);        // 上 3x3
    int leftEmpty = count_empty(map, 3, 0, 6, 3);       // 左 3x3
    int rightEmpty = count_empty(map, 3, 6, 6, 9);      // 右 3x3
    int bottomEmpty = count_empty(map, 6, 3, 9, 6);     // 下 3x3
    //拼入16位辅助信息
    val = (val << 4) | (topEmpty & 0xF);
    val = (val << 4) | (leftEmpty & 0xF);
    val = (val << 4) | (rightEmpty & 0xF);
    val = (val << 4) | (bottomEmpty & 0xF);
    return val;
}

//随机生成地图
void random_map(int res[MAP_SIZE][MAP_SIZE]) {
    //首先将所有元素初始化为9
    for (int i = 0; i < MAP_SIZE; i++) {
        for (int j = 0; j < MAP_SIZE; j++) {
            res[i][j] = EMPTY;
        }
    }
    std::vector<int> nums;
    //将 2~6 每个数字加入 9 次
    for (int i = 2; i <= 6; i++) {
        for (int j = 0; j <= 8; j++) {
            nums.push_back(i);
        }
    }
    //Fisher-Yates 洗牌算法
    std::mt19937 rng(std::time(0)); // 使用Mersenne Twister随机数引擎
    for (int i = nums.size() - 1; i > 0; i--) {
        std::uniform_int_distribution<int> dist(0, i);
        int j = dist(rng); // 随机生成 0 到 i 的索引
        std::swap(nums[j], nums[i]);
    }
    int num = 0;
    //第0-2行中心3列
    for (int i = MAP_START; i < MAP_INDEX_START; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            res[i][j] = nums[num++];
        }
    }
    //第3-5行整行9列
    for (int i = MAP_INDEX_START; i <= MAP_INDEX_END; i++) {
        for (int j = MAP_START; j <= MAP_END; j++) {
            res[i][j] = nums[num++];
        }
    }
    //第6-8行中心3列
    for (int i = MAP_INDEX_END + 1; i < MAP_SIZE; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            res[i][j] = nums[num++];
        }
    }
}

//随机生成地图
static int** random_map() {
    int** map = map_init(EMPTY);
    std::vector<int> nums;
    //将 2~6 每个数字加入 9 次
    for (int i = 2; i <= 6; i++) {
        for (int j = 0; j <= 8; j++) {
            nums.push_back(i);
        }
    }
    //Fisher-Yates 洗牌算法 使用Mersenne Twister随机数引擎
    std::mt19937 rng(std::time(0));
    for (int i = nums.size() - 1; i > 0; i--) {
        std::uniform_int_distribution<int> dist(0, i);
        //随机生成 0 到 i 的索引
        int j = dist(rng);
        std::swap(nums[j], nums[i]);
    }
    int num = 0;
    //第0-2行中心3列
    for (int i = MAP_START; i < MAP_INDEX_START; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            map_set(map, i, j, nums[num++]);
        }
    }
    //第3-5行整行9列
    for (int i = MAP_INDEX_START; i <= MAP_INDEX_END; i++) {
        for (int j = MAP_START; j <= MAP_END; j++) {
            map_set(map, i, j, nums[num++]);
        }
    }
    //第6-8行中心3列
    for (int i = MAP_INDEX_END + 1; i < MAP_SIZE; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            map_set(map, i, j, nums[num++]);
        }
    }
    return map;
}

//转化地图 将地图的2-6转化为计算0/1/EMPTY的1
void convert_map(const int src[MAP_SIZE][MAP_SIZE], int targetMap[MAP_SIZE][MAP_SIZE], int target) {
    for (int i = MAP_START; i < MAP_SIZE; i++) {
        for (int j = MAP_START; j < MAP_SIZE; j++) {
            if (src[i][j] == EMPTY) {
                targetMap[i][j] = EMPTY;
            }
            else if (src[i][j] == target) {
                targetMap[i][j] = 1;
            }
            else {
                targetMap[i][j] = 0;
            }
        }
    }
}
int** convert_map(const int src[MAP_SIZE][MAP_SIZE], int target) {
    int** map = map_init(EMPTY);
    for (int i = MAP_START; i < MAP_SIZE; i++) {
        for (int j = MAP_START; j < MAP_SIZE; j++) {
            if (src[i][j] == EMPTY) {
                map_set(map, i, j, EMPTY);
            }
            else if (src[i][j] == target) {
                map_set(map, i, j, 1);
            }
            else {
                map_set(map, i, j, 0);
            }
        }
    }
    return map;
}
int** convert_map(int** src, int target) {
    int** map = map_init(EMPTY);
    for (int i = MAP_START; i < MAP_SIZE; i++) {
        for (int j = MAP_START; j < MAP_SIZE; j++) {
            if (src[i][j] == EMPTY) {
                map_set(map, i, j, EMPTY);
            }
            else if (src[i][j] == target) {
                map_set(map, i, j, 1);
            }
            else {
                map_set(map, i, j, 0);
            }
        }
    }
    return map;
}

/************** 操作相关方法 **************/

//(操作行/列的索引,0-2,左到右,上到下) 最终索引要 +3
static inline int getOpIndex(int op) {
    //提取高2位:右移2位后取低2位
    return ((op >> 2) & 0b11);
}

//(0[上]1[右]2[下]3[左])
static inline int getOp(int op) {
    //提取低2位:直接取低2位
    return op & 0b11;
}

//选定行左移 row为索引
static int** moveRowLeft(int** map, int row) {
    int** newMap = map_copy(map);
    for (int j = MAP_START; j < MAP_END; j++) {
        map_set(newMap, row, j, map_get(map, row, j + 1));
    }
    map_set(newMap, row, MAP_END, EMPTY);
    return newMap;
}

//判断选定行能否左移
static bool canRowLeft(int** map, int row) {
    return map_get(map, row, MAP_INDEX_END + 1) != EMPTY;
}

//选定行右移 row为索引
static int** moveRowRight(int** map, int row) {
    int** newMap = map_copy(map);
    for (int j = MAP_END; j > MAP_START; j--) {
        map_set(newMap, row, j, map_get(map, row, j - 1));
    }
    map_set(newMap, row, MAP_START, EMPTY);
    return newMap;
}

//判断选定行能否右移
static bool canRowRight(int** map, int row) {
    return map_get(map, row, MAP_INDEX_START - 1) != EMPTY;
}

//选定列上移 col为索引
static int** moveColUp(int** map, int col) {
    int** newMap = map_copy(map);
    for (int i = MAP_START; i < MAP_END; i++) {
        map_set(newMap, i, col, map_get(map, i + 1, col));
    }
    map_set(newMap, MAP_END, col, EMPTY);
    return newMap;
}

//判断选定列能否上移
static bool canColUp(int** map, int col) {
    return map_get(map, MAP_INDEX_END + 1, col) != EMPTY;
}

//选定列下移
static int** moveColDown(int** map, int col) {
    int** newMap = map_copy(map);
    for (int i = MAP_END; i > MAP_START; i--) {
        map_set(newMap, i, col, map_get(map, i - 1, col));
    }
    map_set(newMap, MAP_START, col, EMPTY);
    return newMap;
}

//判断选定列能否下移
static bool canColDown(int** map, int col) {
    return map_get(map, MAP_INDEX_START - 1, col) != EMPTY;
}

/************** 求解相关方法 **************/

//启发函数:中心区域中不等于目标的格子数
static int heuristic(int** map) {
    int mismatch = 0;
    for (int i = MAP_INDEX_START; i <= MAP_INDEX_END; i++) {
        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
            if (map_get(map, i, j) != TARGET) {
                mismatch += 1;
            }
        }
    }
    return mismatch;
}

//剪枝:判断当前操作是否是上一步的反操作
static bool isReverse(const int* path, int pathLen, int move) {
    if (pathLen == 0) {
        return false;
    }
    int last = path[pathLen - 1];
    //操作的索引一样,同时为逆操作14/23
    return ((getOpIndex(last) == getOpIndex(move))) && (getOp(last) + getOp(move) == 3);
}
static bool isReverse(const std::vector<int> path, int move) {
    if (path.empty()) {
        return false;
    }
    int last = path.back();
    //操作的索引一样,同时为逆操作14/23
    bool res = ((getOpIndex(last) == getOpIndex(move))) && (getOp(last) + getOp(move) == 3);
    return res;
}

//计算地图中剩余目标个数
static int target_num(int** map) {
    int num = 0;
    for (int i = MAP_START; i < MAP_SIZE; i++) {
        for (int j = MAP_START; j < MAP_SIZE; j++) {
            if (map_get(map, i, j) == TARGET) {
                num++;
            }
        }
    }
    return num;
}

//节点扩展计数器
static int nodesExpanded = 0;
//记录搜索开始时间
chrono::steady_clock::time_point startTime;
//优先队列,按f值排序
std::priority_queue<CubeMap, std::vector<CubeMap>, std::greater<CubeMap>> pq;
//用于存储已访问状态的压缩序列化字符串
//std::unordered_set<uint64_t> visited;
bloom_filter* visited = nullptr;  // 全局指针存布隆过滤器

//A* 搜索主逻辑
//static CubeMap solve(int startGrid[MAP_SIZE][MAP_SIZE]) {
//    visited->insert(compress(startGrid));
//    // 初始启发值
//    int h0 = heuristic(startGrid);
//
//    int* init[15];
//    for (int i = 0; i < 15; ++i)
//        init[i] = 0;
//
//    pq.push(CubeMap(startGrid, 0, h0, *init, NULL));
//    //重置计数器
//    nodesExpanded = 0;
//    //记录开始时间
//    startTime = chrono::steady_clock::now();
//    while (!pq.empty()) {
//        //取出当前f值最小的状态
//        CubeMap curr = pq.top();
//        pq.pop();
//        //计算过多剪枝 防止内存问题
//        /*if (nodesExpanded >= 1000000) {
//            return {};
//        }*/
//        if (nodesExpanded++ % 100000 == 0) {
//            auto now = chrono::steady_clock::now();
//            auto duration = chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
//            cout << "已计算" << nodesExpanded << "条路径,耗时:" << duration << "ms" << endl;
//        }
//        //如果已达目标状态，返回路径
//        if (curr.h == 0) {
//            //curr.release();
//            return curr;
//        }
//        //步数下限剪枝
//        if (curr.step + curr.h > MAX_DEPTH) {
//            curr.release();
//            continue;
//        }
//        //超过最大步数
//        if (curr.step >= MAX_DEPTH) {
//            curr.release();
//            continue;
//        }
//        //判断总目标是否足够
//        if (curr.targetNum() < 9) {
//            curr.release();
//            continue;
//        }
//        //尝试对第3~5行进行左右移动
//        for (int i = MAP_INDEX_START; i <= MAP_INDEX_END; i++) {
//            //左移
//            if (canRowLeft(curr.map, i)) {
//                //生成操作
//                int op = ((i - 3) << 2) | OP_LEFT;
//                //生成左移后的地图
//                int leftMap[MAP_SIZE][MAP_SIZE];
//                moveRowLeft(curr.map, i, leftMap);
//                uint64_t keyLeft = compress(leftMap);
//                //没有计算左移后的地图,且不是上一步的逆操作
//                if (!visited->contains(keyLeft) && !isReverse(curr.opPath, curr.step, op)) {
//                    visited->insert(keyLeft);
//                    pq.push(CubeMap(leftMap, curr.step + 1, heuristic(leftMap), curr.opPath, op));
//                }
//            }
//            //右移
//            if (canRowRight(curr.map, i)) {
//                int op = ((i - 3) << 2) | OP_RIGHT;
//                int rightMap[MAP_SIZE][MAP_SIZE];
//                moveRowRight(curr.map, i, rightMap);
//                uint64_t keyRight = compress(rightMap);
//                if (!visited->contains(keyRight) && !isReverse(curr.opPath, curr.step, op)) {
//                    visited->insert(keyRight);
//                    pq.push(CubeMap(rightMap, curr.step + 1, heuristic(rightMap), curr.opPath, op));
//                }
//            }
//        }
//        //尝试对第3~5列进行上下移动
//        for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
//            //上移
//            if (canColUp(curr.map, j)) {
//                int op = ((j - 3) << 2) | OP_UP;
//                int upMap[MAP_SIZE][MAP_SIZE];
//                moveColUp(curr.map, j, upMap);
//                uint64_t keyUp = compress(upMap);
//                if (!visited->contains(keyUp) && !isReverse(curr.opPath, curr.step, op)) {
//                    visited->insert(keyUp);
//                    pq.push(CubeMap(upMap, curr.step + 1, heuristic(upMap), curr.opPath, op));
//                }
//            }
//            //下移
//            if (canColDown(curr.map, j)) {
//                int op = ((j - 3) << 2) | OP_DOWN;
//                int downMap[MAP_SIZE][MAP_SIZE];
//                moveColDown(curr.map, j, downMap);
//                uint64_t keyDown = compress(downMap);
//                if (!visited->contains(keyDown) && !isReverse(curr.opPath, curr.step, op)) {
//                    visited->insert(keyDown);
//                    pq.push(CubeMap(downMap, curr.step + 1, heuristic(downMap), curr.opPath, op));
//                }
//            }
//        }
//        curr.release();
//    }
//    //未找到解
//    return CubeMap(startGrid, 0, h0, *init, NULL);
//}

std::vector<std::vector<int>> result_main;
int min_result_step = 999;
std::vector<int> result_cache;

bool dfs(int** map, int step) {
    if (++nodesExpanded % 1000000 == 0) {
        auto now = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
        cout << "已计算" << nodesExpanded << "条路径,耗时:" << duration << "ms" << endl;
    }
    if (step >= min_result_step) {
        return false;
    }
    int h = heuristic(map);
    if (h == 0) {
        if (step < min_result_step) {
            result_main.push_back(result_cache);
            min_result_step = step;
        }
        return true;
    }
    //剩余步数无法满足条件
    if (step + h > MAX_DEPTH) {
        return false;
    }
    //超过最大步数
    if (step >= MAX_DEPTH) {
        return false;
    }
    if (target_num(map) < 9) {
        return false;
    }
    uint64_t key = compress(map);
    if (visited->contains(key)) {
        return false;
    }
    visited->insert(key);
    for (int i = MAP_INDEX_START; i <= MAP_INDEX_END; i++) {
        //左移
        if (canRowLeft(map, i)) {
            int** newMap = moveRowLeft(map, i);
            int op = ((i - 3) << 2) | OP_LEFT;
            if (!isReverse(result_cache, op)) {
                result_cache.push_back(op);
                if (dfs(newMap, step + 1)) {
                    //return false;
                }
                //回溯
                result_cache.pop_back();
            }
            map_delete(newMap);
        }
        //右移
        if (canRowRight(map, i)) {
            int** newMap = moveRowRight(map, i);
            int op = ((i - 3) << 2) | OP_RIGHT;
            if (!isReverse(result_cache, op)) {
                result_cache.push_back(op);
                if (dfs(newMap, step + 1)) {
                    //return false;
                }
                result_cache.pop_back();
            }
            map_delete(newMap);
        }
    }
    for (int j = MAP_INDEX_START; j <= MAP_INDEX_END; j++) {
        //上移
        if (canColUp(map, j)) {
            int** newMap = moveColUp(map, j);
            int op = ((j - 3) << 2) | OP_UP;
            if (!isReverse(result_cache, op)) {
                result_cache.push_back(op);
                if (dfs(newMap, step + 1)) {
                    //return false;
                }
                result_cache.pop_back();
            }
            map_delete(newMap);
        }
        //下移
        if (canColDown(map, j)) {
            int** newMap = moveColDown(map, j);
            int op = ((j - 3) << 2) | OP_DOWN;
            if (!isReverse(result_cache, op)) {
                result_cache.push_back(op);
                if (dfs(newMap, step + 1)) {
                    //return false;
                }
                result_cache.pop_back();
            }
            map_delete(newMap);
        }
    }
    return false;
}

void cube_maze_main() {
    bloom_parameters parameters;
    parameters.projected_element_count = 10000000;
    parameters.false_positive_probability = 0.01;
    parameters.random_seed = 0xA5A5A5A5;
    if (!parameters) {
        std::cerr << "Invalid bloom filter parameters!" << std::endl;
        return;
    }
    parameters.compute_optimal_parameters();
    //释放之前的指针防止内存泄漏
    if (visited) {
        delete visited;
        visited = nullptr;
    }
    visited = new bloom_filter(parameters);
    if (!visited) {
        std::cerr << "参数配置错误" << std::endl;
        return;
    }

    //int map[MAP_SIZE][MAP_SIZE];
    /*int map[MAP_SIZE][MAP_SIZE] = {
        {9, 9, 9, 4, 6, 6, 9, 9, 9},
        {9, 9, 9, 2, 3, 6, 9, 9, 9},
        {9, 9, 9, 3, 4, 2, 9, 9, 9},
        {2, 5, 2, 3, 3, 2, 3, 3, 5},
        {4, 6, 5, 4, 5, 4, 5, 5, 2},
        {2, 3, 3, 4, 6, 5, 4, 2, 5},
        {9, 9, 9, 6, 6, 2, 9, 9, 9},
        {9, 9, 9, 3, 4, 4, 9, 9, 9},
        {9, 9, 9, 5, 6, 6, 9, 9, 9}
    };*/
    int** map = random_map();
    //deCompress(1547278747744, map);
    map_print(map);
    int calPathNum = 0;
    vector<int> resList;
    bool optimal = false;
    auto startTime = chrono::steady_clock::now();
    for (int i = 2; i <= 6; i++) {
        //A*求解
        std::priority_queue<CubeMap, std::vector<CubeMap>, std::greater<CubeMap>>().swap(pq);
        std::vector<CubeMap> baseVec;
        baseVec.reserve(100000);
        std::priority_queue<CubeMap, std::vector<CubeMap>, std::greater<CubeMap>> pq(
            std::greater<CubeMap>(), std::move(baseVec));
        //std::unordered_set<uint64_t>().swap(visited);
        //visited.reserve(1 << 21);// 等价于 reserve(1048576)
        //int calcMap[MAP_SIZE][MAP_SIZE];
        //convert_map(map, calcMap, i);
        //map_print(calcMap);
        //调用solve函数寻找解路径
        //CubeMap result = solve(calcMap);
        //calPathNum += nodesExpanded;
        //cout << "计算路径:" << nodesExpanded << endl;
        //cout << "pq:" << pq.size() << endl;
        //if (result.step > 0) {
        //    cout << "有解!!!!!!!!!" << endl;
        //    for (int i = 0; i < result.step; i++) {
        //        std::cout << result.opPath[i] << " ";
        //    }
        //    std::cout << std::endl;
        //}


        //dfs求解
        int** calcMap = convert_map(map, i);
        std::cout << "开始计算" << std::endl;
        std::cout << compress(calcMap) << std::endl;
        map_print(calcMap);
        std::cout << std::endl;
        dfs(calcMap, 0);
        if (!result_main.empty()) {
            cout << "有解!!!!!!!!!" << result_main.size() << endl;
            break;
        }
    }
    cout << "计算节点数: " << nodesExpanded << endl;
    std::priority_queue<CubeMap, std::vector<CubeMap>, std::greater<CubeMap>>().swap(pq);
    //std::unordered_set<uint64_t>().swap(visited);
    auto endTime = chrono::steady_clock::now();
    if (!result_main.empty()) {
        cout << "共有" << result_main.size() << "种解法" << endl;
        cout << "最佳解法:" << endl;
        std::vector<int> best = result_main.back();
        for (int x : best) {
            std::cout << x << ",";
        }
        std::cout << std::endl;
        for (int op : best) {
            cout << "→ " << op << endl;
            int opIndex = 3 + getOpIndex(op);
            //下面的map就不释放了
            switch (getOp(op)) {
                case OP_UP:
                    cout << "上移第" << (opIndex + 1) << "列" << endl;
                    map = moveColUp(map, opIndex);
                    break;
                case OP_RIGHT:
                    cout << "右移第" << (opIndex + 1) << "行" << endl;
                    map = moveRowRight(map, opIndex);
                    break;
                case OP_DOWN:
                    cout << "下移第" << (opIndex + 1) << "列" << endl;
                    map = moveColDown(map, opIndex);
                    break;
                case OP_LEFT:
                    cout << "左移第" << (opIndex + 1) << "行" << endl;
                    map = moveRowLeft(map, opIndex);
                    break;
                default:
                    break;
            }
            map_print(map);
            cout << endl;
        }
    } else {
        cout << "未能在" << MAX_DEPTH << "步内找到解。" << endl;
    }
    cout << "扩展节点数: " << calPathNum << endl;
    cout << "总耗时: "
        << chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count()
        << " 毫秒" << endl;

}