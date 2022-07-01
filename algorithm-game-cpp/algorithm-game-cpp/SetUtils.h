#pragma once
#include <iostream>
#include <set>

using namespace std;

//释放set所占的内存
void set_delete(set<int>* s);
void set_delete(set<string>* s);