#pragma once
#include <iostream>

using namespace std;

struct CubeMap {
    //垂直的列 1.左 2.中 3.右
    int vCenterIdx1; //0代表第一项
    int vCenterIdx2;
    int vCenterIdx3;
    string vList1;
    string vList2;
    string vList3;
    //水平的列 1.上 2.中 3.下
    int hCenterIdx1; //0代表第一项
    int hCenterIdx2;
    int hCenterIdx3;
    string hList1;
    string hList2;
    string hList3;
};

//一笔画入口函数
void cube_1d_main();