#pragma once
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

//����̨���vector
void vector_print(vector<string> v);

//�ͷ�vector��ռ���ڴ�
void vector_delete(vector<string>* v);

//��vectorת��Ϊjson�ַ���
string vector_2_json_string(vector<string> v);