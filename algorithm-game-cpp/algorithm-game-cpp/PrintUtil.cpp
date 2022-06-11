#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "PrintUtil.h"

using namespace std;

void print_vector(vector<int> v) {
    cout << "---------- print vector<int>" << endl;
    cout << "size:" + to_string(v.size()) << endl;
    for (vector<int>::iterator iter = v.begin(); iter != v.end(); iter++) {
        cout << "\"" + to_string((*iter)) + "\", ";
    }
    cout << endl;
    cout << "----------" << endl;
}

void print_vector(vector<string> v) {
    cout << "---------- print vector<string>" << endl;
    cout << "size:" + to_string(v.size()) << endl;
    for (vector<string>::iterator iter = v.begin(); iter != v.end(); iter++) {
        cout << "\"" + (*iter) + "\", ";
    }
    cout << endl;
    cout << "----------" << endl;
}

void print_map(map<int, vector<string>> m) {
    cout << "---------- print map" << endl;
    cout << "size:" + to_string(m.size()) << endl;
    for (map<int, vector<string>>::iterator iter = m.begin(); iter != m.end(); iter++) {
        cout << "key:" << iter->first << endl;
        print_vector(iter->second);
    }
    cout << "----------" << endl;
}