#pragma once
#include <iostream>
#include <Windows.h>

using namespace std;

//intת16�����ַ���
string int_2_hex_string(int i, int width);

//16�����ַ���ת2�����ַ���(7λ)
string hex_string_2_bin_string(string strHex);

//stringתchar*
char* string_2_char(string str);
char* string_2_char_check(string str);

//wcharתstring
string wchar_2_string(LPCWSTR pwszSrc);