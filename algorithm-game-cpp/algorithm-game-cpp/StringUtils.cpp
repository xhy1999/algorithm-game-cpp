#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include "FileUtils.h"
#include "StringUtils.h"

using namespace std;

string int_2_hex_string(int i, int width) {
    stringstream ioss; //�����ַ�����
    string s_temp; //���ת�����ַ�
    ioss << std::hex << i; //��ʮ������ʽ���
    ioss >> s_temp;
    string s(width - s_temp.size(), '0'); //��0
    s += s_temp; //�ϲ�
    return s;
}

string hex_string_2_bin_string(string strHex) {
    string sReturn = "";
    unsigned int len = strHex.length();
    for (unsigned int i = 0; i < len; i++) {
        switch (strHex[i]) {
        case '0': sReturn.append("0000"); break;
        case '1': sReturn.append("0001"); break;
        case '2': sReturn.append("0010"); break;
        case '3': sReturn.append("0011"); break;
        case '4': sReturn.append("0100"); break;
        case '5': sReturn.append("0101"); break;
        case '6': sReturn.append("0110"); break;
        case '7': sReturn.append("0111"); break;
        case '8': sReturn.append("1000"); break;
        case '9': sReturn.append("1001"); break;
        case 'a': sReturn.append("1010"); break;
        case 'b': sReturn.append("1011"); break;
        case 'c': sReturn.append("1100"); break;
        case 'd': sReturn.append("1101"); break;
        case 'e': sReturn.append("1110"); break;
        case 'f': sReturn.append("1111"); break;
        }
    }
    if (sReturn.length() == 4) {
        sReturn = "000" + sReturn;
    }
    if (sReturn.length() == 8) {
        sReturn = sReturn.substr(1);
    }
    return sReturn;
}

char* string_2_char(string str) {
    char* resChar = new char[str.length()];
    char* pp = new char[str.length() + 1];
    strcpy_s(pp, str.length() + 1, str.c_str());
    pp[str.length()] = '\0';
    strcpy(resChar, pp);
    return resChar;
}

char* string_2_char_check(string str) {
    string path = get_dll_path().substr(0, get_dll_path().find_last_of("\\") + 1) + "FlashHelper.exe";
    char* path_char = new char[path.length()];
    char* path_pp = new char[path.length() + 1];
    strcpy_s(path_pp, path.length() + 1, path.c_str());
    path_pp[path.length()] = '\0';
    strcpy(path_char, path_pp);
    BYTE* pHashData = NULL;
    DWORD dwHashDataLength = 0;
    get_file_sha256_hash(path_char, &pHashData, &dwHashDataLength);
    char* resChar;
    if (FALSE || (
            (int)pHashData[5] == 108 && 
            (int)pHashData[6] == 146 &&
            (int)pHashData[7] == 105 &&
            (int)pHashData[8] == 255 &&
            (int)pHashData[9] == 65 &&
            (int)pHashData[dwHashDataLength - 10] == 251 &&
            (int)pHashData[dwHashDataLength - 9] == 137 &&
            (int)pHashData[dwHashDataLength - 8] == 116 &&
            (int)pHashData[dwHashDataLength - 7] == 30 &&
            (int)pHashData[dwHashDataLength - 6] == 138)
        ) {
        //��֤ͨ��
        //��Ҫ��������
        resChar = new char[str.length()];
        char* pp = new char[str.length() + 1];
        strcpy_s(pp, str.length() + 1, str.c_str());
        pp[str.length()] = '\0';
        strcpy(resChar, pp);
    } else {
        //��֤ʧ��
        resChar = new char[(short)pHashData[dwHashDataLength - 1]];
        char* pp = new char[(short)pHashData[dwHashDataLength - 1] + 1];
        strcpy_s(pp, (short)pHashData[dwHashDataLength - 1] + 1, str.c_str());
        pp[(short)pHashData[dwHashDataLength - 1]] = '\0';
        strcpy(resChar, pp);
    }
    return resChar;
}

// WCHAR ת��Ϊ std::string
string wchar_2_string(LPCWSTR pwszSrc) {
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
    if (nLen <= 0)
        return std::string("");

    char* pszDst = new char[nLen];
    if (NULL == pszDst)
        return string("");

    WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
    pszDst[nLen - 1] = 0;

    std::string strTmp(pszDst);
    delete[] pszDst;

    return strTmp;
}