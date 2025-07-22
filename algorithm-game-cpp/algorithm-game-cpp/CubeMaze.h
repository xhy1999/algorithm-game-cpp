#pragma once
#include <iostream>
#include <vector>
#include "VectorUtils.h"

using namespace std;

//��
const int EMPTY = 9;
//Ŀ������,����3x3�������ɵ�ֵ
const int TARGET = 1;
//��Ŀ��
const int OTHER = 0;

//(0[��]1[��]2[��]3[��])
const int OP_UP = 0;
const int OP_RIGHT = 1;
const int OP_DOWN = 2;
const int OP_LEFT = 3;

//(������/�е�����,0-2,����,�ϵ���)
const int OP_INDEX_3 = 0;
const int OP_INDEX_4 = 1;
const int OP_INDEX_5 = 2;

//��ͼ��СΪ9x9
const int MAP_SIZE = 9;
//��ͼ��ʼ:0
const int MAP_START = 0;
//��ͼ����:MAP_SIZE - 1 = 8
const int MAP_END = MAP_SIZE - 1;
//��ͼ��ʼ����:3
const int MAP_INDEX_START = 3;
//��ͼ��������:5
const int MAP_INDEX_END = 5;
//�����������
const int MAX_DEPTH = 15;

struct CubeMap {

    //��ǰ��ͼ
    int** gameMap;
    //��ǰ����(����㵽��״̬)
    int step;
    //����ֵ(����������Ŀ���ƫ��̶�)
    int h;
    //�����״̬��·����¼,��ʽΪ:����(������/�е�����)+����(0[��]1[��]2[��]3[��])
    //һ��16�������� ��2λ(������/�е�����,0-2,����,�ϵ���) ��2λ(0[��]1[��]2[��]3[��])
    std::vector<int> opPath;

    CubeMap(int** src, int step_, int h_, const std::vector<int>& path) {
        this->step = step_;
        this->h = h_;
        this->opPath = path;
        this->gameMap = map_copy(src);
    }

    void release() {
        std::vector<int>().swap(opPath);
    }

    //A* �㷨�е� f(n) = g(n) + h(n)
    int f() const { return step + h; }

    //��ȡ��ͼ������Ŀ������
    int targetNum() const {
        int num = 0;
        for (int i = MAP_START; i < MAP_SIZE; i++) {
            for (int j = MAP_START; j < MAP_SIZE; j++) {
                if (map_get(gameMap, i, j) == TARGET) {
                    num++;
                }
            }
        }
        return num;
    }

    //��������أ�priority_queue Ĭ����"�󶥶�"
    //Ϊʵ��"С����",������fС������,������ >
    bool operator>(const CubeMap& other) const {
        return this->f() > other.f();
    }

};

void cube_maze_main();