#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include "VectorUtils.h"

using namespace std;

void vector_print(vector<string> v) {
    cout << "---------- print vector<string>" << endl;
    cout << "size:" + to_string(v.size()) << endl;
    cout << vector_2_json_string(v) << endl;
    cout << "----------" << endl;
}

void vector_delete(vector<string>* v) {
    v->erase(v->begin(), v->end());
}

string vector_2_json_string(vector<string> v) {
    string jsonString = "[";
    for (vector<string>::iterator iter = v.begin(); iter != v.end(); iter++) {
        jsonString += "\"" + (*iter) + "\", ";
    }
    return jsonString.substr(0, jsonString.length() - 2) + "]";
}