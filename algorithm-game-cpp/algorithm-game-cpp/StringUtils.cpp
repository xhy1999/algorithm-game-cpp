#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "StringUtils.h"

using namespace std;

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
    char* resChar = new char[str.length() + 10];
    long ld = str.length();
    char* pp = new char[ld + 1];
    strcpy_s(pp, ld + 1, str.c_str());
    pp[ld] = '\0';
    strcpy(resChar, pp);
    return resChar;
}