#include <iostream>
#include <chrono>
#include <string>
#include "ArrayUtils.h"

using namespace std;

void array_print(int** arr) {
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            std::cout << std::to_string(array_get(arr, i, j)) + "\t";
        }
        std::cout << std::endl;
    }
}
void array_print(string** arr) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << array_get(arr, i, j) + "\t";
        }
        std::cout << std::endl;
    }
}

int array_get(int** arr, int var0, int var1) {
    return *(arr[var0] + var1);
}
string array_get(string** arr, int var0, int var1) {
    return *(arr[var0] + var1);
}

void array_set(int** arr, int var0, int var1, int value) {
    arr[var0][var1] = value;
}

int** array_copy(int** arr) {
    int** mainMap = new int* [MAP_HEIGHT_LENGTH];
    mainMap[0] = new int[MAP_WIDTH_LENGTH];
    mainMap[1] = new int[MAP_WIDTH_LENGTH];
    mainMap[2] = new int[MAP_WIDTH_LENGTH];
    mainMap[3] = new int[MAP_WIDTH_LENGTH];
    mainMap[4] = new int[MAP_WIDTH_LENGTH];
    mainMap[5] = new int[MAP_WIDTH_LENGTH];
    mainMap[6] = new int[MAP_WIDTH_LENGTH];
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            mainMap[i][j] = array_get(arr, i, j);
        }
    }
    return mainMap;
}

/*
 * 以下数组宽为2,高为3
 * {{1,2}
 *  {1,2}
 *  {1,2}}
*/
int** array_init(int width, int height) {
    int** arr = new int* [height];
    for (int i = 0; i < height; i++) {
        arr[i] = new int[width];
        for (int j = 0; j < width; j++) {
            arr[i][j] = 0;
        }
    }
    return arr;
}

string** array_init_str(int width, int height) {
    string** arr = new string* [height];
    for (int i = 0; i < height; i++) {
        arr[i] = new string[width];
        for (int j = 0; j < width; j++) {
            arr[i][j] = "";
        }
    }
    return arr;
}

void array_delete(int** arr) {
    for (int i = 0; i < MAP_HEIGHT_LENGTH; ++i) {
        delete[] arr[i];
        arr[i] = NULL;
    }
    delete[] arr;
    arr = NULL;
}
void array_delete(string** arr) {
    for (int i = 0; i < MAP_HEIGHT_LENGTH; ++i) {
        delete[] arr[i];
        arr[i] = NULL;
    }
    delete[] arr;
    arr = NULL;
}

int* array_sort(int* arr) {
    int n = sizeof(arr) / sizeof(int) + 1;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
        }
    }
    return arr;
}

string array_to_string(int** arr) {
    string res;
    for (int i = 0; i < MAP_HEIGHT_LENGTH; i++) {
        for (int j = 0; j < MAP_WIDTH_LENGTH; j++) {
            res += to_string(array_get(arr, i, j));
        }
    }
    return res;
}