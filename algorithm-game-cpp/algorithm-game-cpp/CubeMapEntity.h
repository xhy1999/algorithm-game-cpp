#pragma once
#include <iostream>

using namespace std;


class CubeMapEntity {

    const bool OUT_PUT_TIP = false;

public:

    //��ɫ����
    int colorNum;

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

    // ���캯��
    CubeMapEntity(int colorNum, bool init) {
        this->colorNum = colorNum;
        if (init) {
            this->initMap();
        }
    }

    //��ʼ������
    void initMap() {
        this->vList1 = "132224512";
        this->vList2 = "324321215";
        this->vList3 = "315431222";
        this->hList1 = "231234235";
        this->hList2 = "415223415";
        this->hList3 = "142411315";
        this->vCenterIdx1 = 3;
        this->vCenterIdx2 = 3;
        this->vCenterIdx3 = 3;
        this->hCenterIdx1 = 3;
        this->hCenterIdx2 = 3;
        this->hCenterIdx3 = 3;
        this->syncCenter3x3(true);
    }

    //ͬ���м�3x3����
    void syncCenter3x3(bool byV) {
        if (byV) {
            //������Ϊ׼
            this->hList1[this->hCenterIdx1] = this->vList1[this->vCenterIdx1];
            this->hList2[this->hCenterIdx2] = this->vList1[this->vCenterIdx1 + 1];
            this->hList3[this->hCenterIdx3] = this->vList1[this->vCenterIdx1 + 2];
            this->hList1[this->hCenterIdx1 + 1] = this->vList2[this->vCenterIdx2];
            this->hList2[this->hCenterIdx2 + 1] = this->vList2[this->vCenterIdx2 + 1];
            this->hList3[this->hCenterIdx3 + 1] = this->vList2[this->vCenterIdx2 + 2];
            this->hList1[this->hCenterIdx1 + 2] = this->vList3[this->vCenterIdx3];
            this->hList2[this->hCenterIdx2 + 2] = this->vList3[this->vCenterIdx3 + 1];
            this->hList3[this->hCenterIdx3 + 2] = this->vList3[this->vCenterIdx3 + 2];
        } else {
            //�Ժ���Ϊ׼
            this->vList1[this->vCenterIdx1] = hList1[this->hCenterIdx1];
            this->vList2[this->vCenterIdx2] = hList1[this->hCenterIdx1 + 1];
            this->vList3[this->vCenterIdx3] = hList1[this->hCenterIdx1 + 2];
            this->vList1[this->vCenterIdx1 + 1] = hList2[this->hCenterIdx2];
            this->vList2[this->vCenterIdx2 + 1] = hList2[this->hCenterIdx2 + 1];
            this->vList3[this->vCenterIdx3 + 1] = hList2[this->hCenterIdx2 + 2];
            this->vList1[this->vCenterIdx1 + 2] = hList2[this->hCenterIdx3];
            this->vList2[this->vCenterIdx2 + 2] = hList2[this->hCenterIdx3 + 1];
            this->vList3[this->vCenterIdx3 + 2] = hList2[this->hCenterIdx3 + 2];
        }
    }

    // ����Ƿ�������Ϸ��������:�м�3x3������ɫһ��
    bool isGoalState() {
        char color = this->hList1[this->hCenterIdx1];
        return color == this->hList1[this->hCenterIdx1 + 1] && color == this->hList1[this->hCenterIdx1 + 2] &&
            color == this->hList2[this->hCenterIdx2] && color == this->hList2[this->hCenterIdx2 + 1] && color == this->hList2[this->hCenterIdx2 + 2] &&
            color == this->hList3[this->hCenterIdx3] && color == this->hList3[this->hCenterIdx3 + 1] && color == this->hList3[this->hCenterIdx3 + 2];
    }

};

