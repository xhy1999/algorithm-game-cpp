#pragma once
#include <iostream>

using namespace std;

struct CubeMap {
    //��ֱ���� 1.�� 2.�� 3.��
    int vCenterIdx1; //0�����һ��
    int vCenterIdx2;
    int vCenterIdx3;
    string vList1;
    string vList2;
    string vList3;
    //ˮƽ���� 1.�� 2.�� 3.��
    int hCenterIdx1; //0�����һ��
    int hCenterIdx2;
    int hCenterIdx3;
    string hList1;
    string hList2;
    string hList3;
};

//һ�ʻ���ں���
void cube_1d_main();