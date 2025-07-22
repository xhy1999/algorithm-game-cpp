#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include "VectorUtils.h"

using namespace std;

void vector_print(vector<string> v) {
    cout << "---------- print vector<string>" << endl;
    cout << "size:" + std::to_string(v.size()) << endl;
    for (vector<string>::iterator iter = v.begin(); iter != v.end(); iter++) {
        cout << "\"" + (*iter) + "\", ";
    }
    cout << endl;
    cout << "----------" << endl;
}

void vector_print(vector<int> v) {
    cout << "---------- print vector<int>" << endl;
    cout << "size:" + std::to_string(v.size()) << endl;
    for (int item : v) {
        std::cout << item << ", ";
    }
    cout << endl;
    cout << "----------" << endl;
}

void vector_delete(std::vector<int>* v) {
    std::vector<int>().swap(*v);
}

void vector_delete(vector<string>* v) {
    v->erase(v->begin(), v->end());
}

void vector_delete(vector<MapInfo>* v) {
    v->erase(v->begin(), v->end());
}

string vector_2_json_string(vector<string> v) {
    string jsonString = "[";
    for (vector<string>::iterator iter = v.begin(); iter != v.end(); iter++) {
        jsonString += "\'" + (*iter) + "\', ";
    }
    return jsonString.substr(0, jsonString.length() - 2) + "]";
}