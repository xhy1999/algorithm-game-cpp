#pragma once
#include <iostream>
#include <Windows.h>

using namespace std;

//int转16进制字符串
string int_2_hex_string(int i, int width);

//16进制字符串转2进制字符串(7位)
string hex_string_2_bin_string(string strHex);

//string转char*
char* string_2_char(string str);
char* string_2_char_check(string str);

//wchar转string
string wchar_2_string(LPCWSTR pwszSrc);