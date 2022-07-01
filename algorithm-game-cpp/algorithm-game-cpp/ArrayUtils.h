#pragma once
#include <iostream>

using namespace std;

#define MAP_WIDTH_LENGTH 7
#define MAP_HEIGHT_LENGTH 7

//����̨�����ά����
void array_print(int** arr);

//��ȡ��ά����ĳ���ֵ
int array_get(int** arr, int var0, int var1);

//���ö�ά����ĳ��
void array_set(int** arr, int var0, int var1, int value);

//���һ����ά����
int** array_copy(int** arr);

//��ʼ��һ������Ķ�ά����,��Ĭ��ֵΪ0
int** array_init(int width, int height);

//�ͷŶ�ά������ռ�ڴ�
void array_delete(int** arr);

//��һ��һά�����С��������
int* array_sort(int* arr);

//����ά����תΪ�ַ���
string array_to_string(int** arr);