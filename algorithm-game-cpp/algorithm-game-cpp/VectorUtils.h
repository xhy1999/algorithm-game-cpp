#pragma once
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

//控制台输出vector
void vector_print(vector<string> v);

//释放vector所占的内存
void vector_delete(vector<string>* v);

//将vector转化为json字符串
string vector_2_json_string(vector<string> v);