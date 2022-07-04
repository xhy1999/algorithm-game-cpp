#pragma once
#include <iostream>
#include <Windows.h>

using namespace std;

//16进制字符串转2进制字符串(7位)
string hex_string_2_bin_string(string strHex);

//string转char*
char* string_2_char(string str);

//wchar转string
string wchar_2_string(LPCWSTR pwszSrc);