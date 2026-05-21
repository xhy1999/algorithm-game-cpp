#pragma once
#include <iostream>
#include <windows.h>

using namespace std;

//读取文件
BOOL get_file_data(const wchar_t* pszFilePath, BYTE** ppFileData, DWORD* pdwFileDataLength);

//获取文件hash值
BOOL calculate_hash(BYTE* pData, DWORD dwDataLength, ALG_ID algHashType, BYTE** ppHashData, DWORD* pdwHashDataLength);

//获取文件sha256特征值
BOOL get_file_sha256_hash(const wchar_t* pszFilePath, BYTE** pHashData, DWORD* dwHashDataLength);

//获取当前dll路径
std::wstring get_dll_path();