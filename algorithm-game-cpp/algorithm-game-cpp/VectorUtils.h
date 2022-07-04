#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include "OneLine.h"

using namespace std;

//控制台输出vector
void vector_print(vector<string> v);

//释放vector所占的内存
void vector_delete(vector<string>* v);

//释放vector所占的内存
void vector_delete(vector<MapInfo>* v);

//将vector转化为json字符串
string vector_2_json_string(vector<string> v);