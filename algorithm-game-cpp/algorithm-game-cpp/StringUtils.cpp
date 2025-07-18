#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include "FileUtils.h"
#include "StringUtils.h"
#include "RandomUtils.h"

using namespace std;

string int_2_hex_string(int i, int width) {
    std::stringstream ioss; //定义字符串流
    std::string s_temp; //存放转化后字符
    ioss << std::hex << i; //以十六制形式输出
    ioss >> s_temp;
    if (width > s_temp.size()) {
        std::string s_0(width - s_temp.size(), '0'); //位数不够则补0
        s_temp = s_0 + s_temp; //合并
    }
    std::string s = s_temp.substr(s_temp.length() - width, s_temp.length()); //取右width位
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

char* string_2_char_fail(string str) {
    int length = ran_int(0xD0, 0x100);
    char* resChar = new char[length];
    char* pp = new char[length + 1];
    strcpy_s(pp, length + 1, str.c_str());
    pp[length] = '\0';
    strcpy(resChar, pp);
    return resChar;
}

char* string_2_char_check(string str) {
    string path = ""; //get_dll_path().substr(0, get_dll_path().find_last_of("\\") + 1) + "FlashHelper.exe";
    char* path_char = new char[path.length()];
    char* path_pp = new char[path.length() + 1];
    strcpy_s(path_pp, path.length() + 1, path.c_str());
    path_pp[path.length()] = '\0';
    strcpy(path_char, path_pp);
    BYTE* pHashData = NULL;
    DWORD dwHashDataLength = 0;
    //get_file_sha256_hash(path_char, &pHashData, &dwHashDataLength);
    char* resChar;
    if (FALSE || (
        int_2_hex_string((int)pHashData[1], 2) == "b7" &&
        int_2_hex_string((int)pHashData[3], 2) == "60" &&
        int_2_hex_string((int)pHashData[5], 2) == "2f" &&
        int_2_hex_string((int)pHashData[7], 2) == "73" &&
        int_2_hex_string((int)pHashData[9], 2) == "7e" &&
        int_2_hex_string((int)pHashData[dwHashDataLength - 10], 2) == "81" &&
        int_2_hex_string((int)pHashData[dwHashDataLength - 8], 2) == "a6" &&
        int_2_hex_string((int)pHashData[dwHashDataLength - 6], 2) == "05" &&
        int_2_hex_string((int)pHashData[dwHashDataLength - 4], 2) == "c6" &&
        int_2_hex_string((int)pHashData[dwHashDataLength - 2], 2) == "c5")
        ) {
        //验证通过
        //主要拷贝方法
        resChar = new char[str.length()];
        char* pp = new char[str.length() + 1];
        strcpy_s(pp, str.length() + 1, str.c_str());
        pp[str.length()] = '\0';
        strcpy(resChar, pp);
    }
    else {
        //验证失败
        resChar = new char[(int)pHashData[dwHashDataLength - 1]];
        char* pp = new char[(int)pHashData[dwHashDataLength - 1] + 1];
        strcpy_s(pp, (int)pHashData[dwHashDataLength - 1] + 1, str.c_str());
        pp[(short)pHashData[dwHashDataLength - 1]] = '\0';
        strcpy(resChar, pp);
    }
    return resChar;
}

// WCHAR 转换为 std::string
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