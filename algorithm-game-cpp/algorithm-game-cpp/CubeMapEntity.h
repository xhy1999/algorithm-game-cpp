#pragma once
#include <iostream>
#include "ArrayUtils.h"

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

    string op;

    CubeMapEntity(int colorNum, bool init);

    // ������캯��
    CubeMapEntity(const CubeMapEntity& other);
    CubeMapEntity& operator=(const CubeMapEntity& other) {
        if (this != &other) {
            this->colorNum = other.colorNum;
            this->vCenterIdx1 = other.vCenterIdx1;
            this->vCenterIdx2 = other.vCenterIdx2;
            this->vCenterIdx3 = other.vCenterIdx3;
            this->vList1 = other.vList1;
            this->vList2 = other.vList2;
            this->vList3 = other.vList3;
            this->hCenterIdx1 = other.hCenterIdx1;
            this->hCenterIdx2 = other.hCenterIdx2;
            this->hCenterIdx3 = other.hCenterIdx3;
            this->hList1 = other.hList1;
            this->hList2 = other.hList2;
            this->hList3 = other.hList3;
            this->op = other.op;
        }
        return *this;
    }

    string strHashCode();

    //��ʼ������
    void initMap();

    //ͬ���м�3x3����
    void syncCenter3x3(bool byV);

    //����Ƿ�������Ϸ��������:�м�3x3������ɫһ��
    bool isGoalState();

    //******************** �������� ********************//

    //�������ֽ��
    bool upLeft(bool doOp);
    //�����м�ֽ��
    bool upMiddle(bool doOp);
    //�����ұ�ֽ��
    bool upRight(bool doOp);
    //�������ֽ��
    bool downLeft(bool doOp);
    //�����м�ֽ��
    bool downMiddle(bool doOp);
    //�����ұ�ֽ��
    bool downRight(bool doOp);
    //�����ϱ�ֽ��
    bool leftUp(bool doOp);
    //�����м�ֽ��
    bool leftMiddle(bool doOp);
    //�����±�ֽ��
    bool leftDown(bool doOp);
    //�����ϱ�ֽ��
    bool rightUp(bool doOp);
    //�����м�ֽ��
    bool rightMiddle(bool doOp);
    //�����±�ֽ��
    bool rightDown(bool doOp);
    /**
     * ��ӡ��ͼ
     * @param verticalShowNum   ���¶���ʾ������
     * @param horizontalShowNum ���Ҷ���ʾ������
     */
    void printMap(int verticalShowNum, int horizontalShowNum);
    void printMap() { this->printMap(3, 3); }
    //��ȡ�м�3x3����,ע��,��λ������Ҫ�ֶ��ͷ��ڴ�
    string** getCenter3x3V();
    string getCenter3x3VStr();
    string** getCenter3x3H();
    //��ӡ�м�3x3����
    void printCenter3x3();

};

