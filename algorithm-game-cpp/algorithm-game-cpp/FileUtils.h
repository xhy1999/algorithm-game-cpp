#pragma once
#include <iostream>
#include <windows.h>

using namespace std;

//��ȡ�ļ�
BOOL get_file_data(const char* pszFilePath, BYTE** ppFileData, DWORD* pdwFileDataLength);

//��ȡ�ļ�hashֵ
BOOL calculate_hash(BYTE* pData, DWORD dwDataLength, ALG_ID algHashType, BYTE** ppHashData, DWORD* pdwHashDataLength);

//��ȡ�ļ�sha256����ֵ
//BOOL get_file_sha256_hash(const char* pszFilePath, BYTE** pHashData, DWORD* dwHashDataLength);

//��ȡ��ǰdll·��
//string get_dll_path();