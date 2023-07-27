#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include "ArrayUtils.h"
#include "CubeMapEntity.h"

using namespace std;

// 构造函数
CubeMapEntity::CubeMapEntity(int colorNum, bool init) {
    this->colorNum = colorNum;
    if (init) {
        this->initMap();
    }
}

CubeMapEntity::CubeMapEntity(const CubeMapEntity& other) {
    CubeMapEntity copy = CubeMapEntity(this->colorNum, false);
    copy.vCenterIdx1 = this->vCenterIdx1;
    copy.vCenterIdx2 = this->vCenterIdx2;
    copy.vCenterIdx3 = this->vCenterIdx3;
    copy.vList1 = this->vList1;
    copy.vList2 = this->vList2;
    copy.vList3 = this->vList3;
    copy.hCenterIdx1 = this->hCenterIdx1;
    copy.hCenterIdx2 = this->hCenterIdx2;
    copy.hCenterIdx3 = this->hCenterIdx3;
    copy.hList1 = this->hList1;
    copy.hList2 = this->hList2;
    copy.hList3 = this->hList3;
    copy.op = this->op;
}

string CubeMapEntity::strHashCode() {
    //因为地图都一样,所以这里只看index
    return to_string(this->vCenterIdx1) + to_string(this->vCenterIdx2) + to_string(this->vCenterIdx3) +
        to_string(this->hCenterIdx1) + to_string(this->hCenterIdx2) + to_string(this->hCenterIdx3) + this->getCenter3x3VStr();
}

//初始化棋盘
void CubeMapEntity::initMap() {
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

//同步中间3x3棋盘
void CubeMapEntity::syncCenter3x3(bool byV) {
    if (byV) {
        //以纵向为准
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
        //以横向为准
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

// 检查是否满足游戏结束条件:中间3x3部分颜色一致
bool CubeMapEntity::isGoalState() {
    char color = this->hList1[this->hCenterIdx1];
    return color == this->hList1[this->hCenterIdx1 + 1] && 
        color == this->hList1[this->hCenterIdx1 + 2] &&
        color == this->hList2[this->hCenterIdx2] && 
        color == this->hList2[this->hCenterIdx2 + 1] && 
        color == this->hList2[this->hCenterIdx2 + 2] &&
        color == this->hList3[this->hCenterIdx3] && 
        color == this->hList3[this->hCenterIdx3 + 1] && 
        color == this->hList3[this->hCenterIdx3 + 2];
}


//******************** 操作函数 ********************//

//上拉左边纸带
bool CubeMapEntity::upLeft(bool doOp) {
    //判断是否越界
    if (this->vCenterIdx1 + 3 >= this->vList1.length()) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:左边纸带无法上拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->vCenterIdx1++;
        this->hList1[hCenterIdx1] = this->vList1[this->vCenterIdx1];
        this->hList2[hCenterIdx2] = this->vList1[this->vCenterIdx1 + 1];
        this->hList3[hCenterIdx3] = this->vList1[this->vCenterIdx1 + 2];
        this->op.append("ul");
    }
    return true;
}
//上拉中间纸带
bool CubeMapEntity::upMiddle(bool doOp) {
    if (this->vCenterIdx2 + 3 >= this->vList2.length()) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:中间纸带无法上拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->vCenterIdx2++;
        this->hList1[hCenterIdx1 + 1] = this->vList2[this->vCenterIdx2];
        this->hList2[hCenterIdx2 + 1] = this->vList2[this->vCenterIdx2 + 1];
        this->hList3[hCenterIdx3 + 1] = this->vList2[this->vCenterIdx2 + 2];
        this->op.append("um");
    }
    return true;
}
//上拉右边纸带
bool CubeMapEntity::upRight(bool doOp) {
    if (this->vCenterIdx3 + 3 >= this->vList3.length()) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:右边纸带无法上拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->vCenterIdx3++;
        this->hList1[hCenterIdx1 + 2] = this->vList3[this->vCenterIdx3];
        this->hList2[hCenterIdx2 + 2] = this->vList3[this->vCenterIdx3 + 1];
        this->hList3[hCenterIdx3 + 2] = this->vList3[this->vCenterIdx3 + 2];
        this->op.append("ur");
    }
    return true;
}
//下拉左边纸带
bool CubeMapEntity::downLeft(bool doOp) {
    if (this->vCenterIdx1 == 0) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:左边纸带无法下拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->vCenterIdx1--;
        this->hList1[hCenterIdx1] = this->vList1[this->vCenterIdx1];
        this->hList2[hCenterIdx2] = this->vList1[this->vCenterIdx1 + 1];
        this->hList3[hCenterIdx3] = this->vList1[this->vCenterIdx1 + 2];
        this->op.append("dl");
    }
    return true;
}
//下拉中间纸带
bool CubeMapEntity::downMiddle(bool doOp) {
    if (this->vCenterIdx2 == 0) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:左边纸带无法下拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->vCenterIdx2--;
        this->hList1[hCenterIdx1 + 1] = this->vList2[this->vCenterIdx2];
        this->hList2[hCenterIdx2 + 1] = this->vList2[this->vCenterIdx2 + 1];
        this->hList3[hCenterIdx3 + 1] = this->vList2[this->vCenterIdx2 + 2];
        this->op.append("dm");
    }
    return true;
}
//下拉右边纸带
bool CubeMapEntity::downRight(bool doOp) {
    if (this->vCenterIdx3 == 0) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:右边纸带无法下拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->vCenterIdx3--;
        this->hList1[hCenterIdx1 + 2] = this->vList3[this->vCenterIdx3];
        this->hList2[hCenterIdx2 + 2] = this->vList3[this->vCenterIdx3 + 1];
        this->hList3[hCenterIdx3 + 2] = this->vList3[this->vCenterIdx3 + 2];
        this->op.append("dr");
    }
    return true;
}
//左拉上边纸带
bool CubeMapEntity::leftUp(bool doOp) {
    if (this->hCenterIdx1 + 3 >= this->hList1.length()) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:上边纸带无法左拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->hCenterIdx1++;
        this->vList1[vCenterIdx1] = this->hList1[this->hCenterIdx1];
        this->vList2[vCenterIdx2] = this->hList1[this->hCenterIdx1 + 1];
        this->vList3[vCenterIdx3] = this->hList1[this->hCenterIdx1 + 2];
        this->op.append("lu");
    }
    return true;
}
//左拉中间纸带
bool CubeMapEntity::leftMiddle(bool doOp) {
    if (this->hCenterIdx2 + 3 >= this->hList2.length()) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:中间纸带无法左拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->hCenterIdx2++;
        this->vList1[vCenterIdx1 + 1]= this->hList2[this->hCenterIdx2];
        this->vList2[vCenterIdx2 + 1]= this->hList2[this->hCenterIdx2 + 1];
        this->vList3[vCenterIdx3 + 1]= this->hList2[this->hCenterIdx2 + 2];
        this->op.append("lm");
    }
    return true;
}
//左拉下边纸带
bool CubeMapEntity::leftDown(bool doOp) {
    if (this->hCenterIdx3 + 3 >= this->hList3.length()) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:下边纸带无法左拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->hCenterIdx3++;
        this->vList1[vCenterIdx1 + 2] = this->hList3[this->hCenterIdx3];
        this->vList2[vCenterIdx2 + 2] = this->hList3[this->hCenterIdx3 + 1];
        this->vList3[vCenterIdx3 + 2] = this->hList3[this->hCenterIdx3 + 2];
        this->op.append("ld");
    }
    return true;
}
//右拉上边纸带
bool CubeMapEntity::rightUp(bool doOp) {
    if (this->hCenterIdx1 == 0) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:上边纸带无法右拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->hCenterIdx1--;
        this->vList1[vCenterIdx1] = this->hList1[this->hCenterIdx1];
        this->vList2[vCenterIdx2] = this->hList1[this->hCenterIdx1 + 1];
        this->vList3[vCenterIdx3] = this->hList1[this->hCenterIdx1 + 2];
        this->op.append("ru");
    }
    return true;
}
//右拉中间纸带
bool CubeMapEntity::rightMiddle(bool doOp) {
    if (this->hCenterIdx2 == 0) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:中间纸带无法右拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->hCenterIdx2--;
        this->vList1[vCenterIdx1 + 1] = this->hList2[this->hCenterIdx2];
        this->vList2[vCenterIdx2 + 1] = this->hList2[this->hCenterIdx2 + 1];
        this->vList3[vCenterIdx3 + 1] = this->hList2[this->hCenterIdx2 + 2];
        this->op.append("rm");
    }
    return true;
}
//右拉下边纸带
bool CubeMapEntity::rightDown(bool doOp) {
    if (this->hCenterIdx3 == 0) {
        if (CubeMapEntity::OUT_PUT_TIP) {
            cout << "操作失败:下边纸带无法右拉" << endl;
        }
        return false;
    }
    if (doOp) {
        this->hCenterIdx3--;
        this->vList1[vCenterIdx1 + 2] = this->hList3[this->hCenterIdx3];
        this->vList2[vCenterIdx2 + 2] = this->hList3[this->hCenterIdx3 + 1];
        this->vList3[vCenterIdx3 + 2] = this->hList3[this->hCenterIdx3 + 2];
        this->op.append("rd");
    }
    return true;
}
/**
 * 打印地图
 * @param verticalShowNum   上下多显示的数量
 * @param horizontalShowNum 左右多显示的数量
 */
void CubeMapEntity::printMap(int verticalShowNum, int horizontalShowNum) {
    //cout << "vList1:" + this->vList1 << endl;
    //cout << "vList2:" + this->vList2 << endl;
    //cout << "vList3:" + this->vList3 << endl;
    //cout << "hList1:" + this->hList1 << endl;
    //cout << "hList2:" + this->hList2 << endl;
    //cout << "hList3:" + this->hList3 << endl;
    string** printMap = array_init_str(3 + horizontalShowNum * 2, 3 + verticalShowNum * 2);
    //先填入竖直部分
    for (int i = 0; i < 3 + verticalShowNum * 2; i++) {
        int index1 = this->vCenterIdx1 - verticalShowNum + i;
        if (index1 > -1 && index1 < this->vList1.length()) {
            printMap[i][horizontalShowNum] = this->vList1[index1];
        }
        int index2 = this->vCenterIdx2 - verticalShowNum + i;
        if (index2 > -1 && index2 < this->vList2.length()) {
            printMap[i][horizontalShowNum + 1] = this->vList2[index2];
        }
        int index3 = this->vCenterIdx3 - verticalShowNum + i;
        if (index3 > -1 && index3 < this->vList3.length()) {
            printMap[i][horizontalShowNum + 2] = this->vList3[index3];
        }
    }
    //再填入水平部分
    for (int i = 0; i < 3 + horizontalShowNum * 2; i++) {
        int index1 = this->hCenterIdx1 - horizontalShowNum + i;
        if (index1 > -1 && index1 < this->hList1.length()) {
            printMap[verticalShowNum][i] = this->hList1[index1];
        }
        int index2 = this->hCenterIdx2 - horizontalShowNum + i;
        if (index2 > -1 && index2 < this->hList2.length()) {
            printMap[verticalShowNum + 1][i] = this->hList2[index2];
        }
        int index3 = this->hCenterIdx3 - horizontalShowNum + i;
        if (index3 > -1 && index3 < this->hList3.length()) {
            printMap[verticalShowNum + 2][i] = this->hList3[index3];
        }
    }
    array_print(printMap);
    array_delete(printMap);
}
//获取中间3x3棋盘
string** CubeMapEntity::getCenter3x3V() {
    string** arrayV = array_init_str(3, 3);
    for (int i = 0; i < 3; i++) {
        arrayV[i][0] = vList1[vCenterIdx1 + i];
        arrayV[i][1] = vList2[vCenterIdx2 + i];
        arrayV[i][2] = vList3[vCenterIdx3 + i];
    }
    return arrayV;
}
string CubeMapEntity::getCenter3x3VStr() {
    string res = "";
    for (int i = 0; i < 3; i++) {
        res += vList1[vCenterIdx1 + i];
        res += vList2[vCenterIdx2 + i];
        res += vList3[vCenterIdx3 + i];
    }
    return res;
}
string** CubeMapEntity::getCenter3x3H() {
    string** arrayH = array_init_str(3, 3);
    arrayH[0][0] = hList1[hCenterIdx1];
    arrayH[0][1] = hList1[hCenterIdx1 + 1];
    arrayH[0][2] = hList1[hCenterIdx1 + 2];
    arrayH[1][0] = hList2[hCenterIdx2];
    arrayH[1][1] = hList2[hCenterIdx2 + 1];
    arrayH[1][2] = hList2[hCenterIdx2 + 2];
    arrayH[2][0] = hList3[hCenterIdx3];
    arrayH[2][1] = hList3[hCenterIdx3 + 1];
    arrayH[2][2] = hList3[hCenterIdx3 + 2];
    return arrayH;
}
void CubeMapEntity::printCenter3x3() {
    cout << "V:" << endl;
    string** arrayV = getCenter3x3V();
    array_print(arrayV);
    array_delete(arrayV);
    string** arrayH = getCenter3x3H();
    cout << "H:" << endl;
    array_print(arrayH);
    array_delete(arrayH);
}


