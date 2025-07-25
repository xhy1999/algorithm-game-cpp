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
const int OP_LEFT = 2;
const int OP_DOWN = 3;

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
    int map[MAP_SIZE][MAP_SIZE];
    //��ǰ����(����㵽��״̬)
    int step;
    //����ֵ(����������Ŀ���ƫ��̶�)
    int h;
    //�����״̬��·����¼,��ʽΪ:����(������/�е�����)+����(0[��]1[��]2[��]3[��])
    //һ��16�������� ��2λ(������/�е�����,0-2,����,�ϵ���) ��2λ(0[��]1[��]2[��]3[��])
    //std::vector<int> opPath;
    //�����״̬��·����¼
    int* opPath;

    CubeMap(const int src[9][9], int step_, int h_, const int* path_, int newOp) {
        //������ͼ
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                this->map[i][j] = src[i][j];
            }
        }
        this->step = step_;
        this->h = h_;
        //��������
        opPath = new int[15];
        //���ƾ�·��
        for (int i = 0; i < this->step && i < 15; i++) {
            opPath[i] = path_[i];
        }
        if (newOp != NULL) {
            opPath[this->step] = newOp;
        }
    }

    void release() {
        //std::vector<int>().swap(opPath);
        delete[] opPath;
        opPath = nullptr;
    }

    //A* �㷨�е� f(n) = g(n) + h(n)
    int f() const { return step + h; }

    //��ȡ��ͼ������Ŀ������
    int targetNum() const {
        int num = 0;
        for (int i = MAP_START; i < MAP_SIZE; i++) {
            for (int j = MAP_START; j < MAP_SIZE; j++) {
                if (map[i][j] == TARGET) {
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