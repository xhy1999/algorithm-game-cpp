#pragma once
#include <iostream>

using namespace std;

#define MAP_WIDTH_LENGTH 7
#define MAP_HEIGHT_LENGTH 7

//控制台输出二维数组
void array_print(int** arr);

//获取二维数组某项的值
int array_get(int** arr, int var0, int var1);

//设置二维数组某项
void array_set(int** arr, int var0, int var1, int value);

//深拷贝一个二维数组
int** array_copy(int** arr);

//初始化一个长宽的二维数组,且默认值为0
int** array_init(int width, int height);

//释放二维数组所占内存
void array_delete(int** arr);

//将一个一维数组从小到大排序
int* array_sort(int* arr);

//将二维数组转为字符串
string array_to_string(int** arr);