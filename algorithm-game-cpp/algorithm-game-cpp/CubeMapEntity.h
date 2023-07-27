#pragma once
#include <iostream>
#include "ArrayUtils.h"

using namespace std;


class CubeMapEntity {

    const bool OUT_PUT_TIP = false;

public:

    //颜色数量
    int colorNum;
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

    string op;

    CubeMapEntity(int colorNum, bool init);

    // 深拷贝构造函数
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

    //初始化棋盘
    void initMap();

    //同步中间3x3棋盘
    void syncCenter3x3(bool byV);

    //检查是否满足游戏结束条件:中间3x3部分颜色一致
    bool isGoalState();

    //******************** 操作函数 ********************//

    //上拉左边纸带
    bool upLeft(bool doOp);
    //上拉中间纸带
    bool upMiddle(bool doOp);
    //上拉右边纸带
    bool upRight(bool doOp);
    //下拉左边纸带
    bool downLeft(bool doOp);
    //下拉中间纸带
    bool downMiddle(bool doOp);
    //下拉右边纸带
    bool downRight(bool doOp);
    //左拉上边纸带
    bool leftUp(bool doOp);
    //左拉中间纸带
    bool leftMiddle(bool doOp);
    //左拉下边纸带
    bool leftDown(bool doOp);
    //右拉上边纸带
    bool rightUp(bool doOp);
    //右拉中间纸带
    bool rightMiddle(bool doOp);
    //右拉下边纸带
    bool rightDown(bool doOp);
    /**
     * 打印地图
     * @param verticalShowNum   上下多显示的数量
     * @param horizontalShowNum 左右多显示的数量
     */
    void printMap(int verticalShowNum, int horizontalShowNum);
    void printMap() { this->printMap(3, 3); }
    //获取中间3x3棋盘,注意,二位数组需要手动释放内存
    string** getCenter3x3V();
    string getCenter3x3VStr();
    string** getCenter3x3H();
    //打印中间3x3棋盘
    void printCenter3x3();

};

