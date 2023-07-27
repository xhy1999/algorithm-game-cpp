//#include <iostream>
//#include <windows.h>
//
//using namespace std;
//
//BOOL get_file_data(const char* pszFilePath, BYTE** ppFileData, DWORD* pdwFileDataLength) {
//    BOOL bRet = TRUE;
//    BYTE* pFileData = NULL;
//    DWORD dwFileDataLength = 0;
//    HANDLE hFile = NULL;
//    DWORD dwTemp = 0;
//    do {
//        hFile = ::CreateFile(pszFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
//        if (INVALID_HANDLE_VALUE == hFile) {
//            bRet = FALSE;
//            //cout << "FALSE1" << endl;
//            break;
//        }
//        dwFileDataLength = ::GetFileSize(hFile, NULL);
//        pFileData = new BYTE[dwFileDataLength];
//        if (NULL == pFileData) {
//            bRet = FALSE;
//            //cout << "FALSE2" << endl;
//            break;
//        }
//        ::RtlZeroMemory(pFileData, dwFileDataLength);
//        if (::ReadFile(hFile, pFileData, dwFileDataLength, &dwTemp, NULL));
//        // 返回
//        *ppFileData = pFileData;
//        *pdwFileDataLength = dwFileDataLength;
//    } while (FALSE);
//    if (hFile) {
//        ::CloseHandle(hFile);
//    }
//    return bRet;
//}
//
//BOOL calculate_hash(BYTE* pData, DWORD dwDataLength, ALG_ID algHashType, BYTE** ppHashData, DWORD* pdwHashDataLength) {
//    HCRYPTPROV hCryptProv = NULL;
//    HCRYPTHASH hCryptHash = NULL;
//    BYTE* pHashData = NULL;
//    DWORD dwHashDataLength = 0;
//    DWORD dwTemp = 0;
//    BOOL bRet = FALSE;
//    do {
//        // 获得指定CSP的密钥容器的句柄
//        bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
//        if (FALSE == bRet) {
//            //cout << "calculate_hash FALSE1" << endl;
//            break;
//        }
//        // 创建一个HASH对象, 指定HASH算法
//        bRet = ::CryptCreateHash(hCryptProv, algHashType, NULL, NULL, &hCryptHash);
//        if (FALSE == bRet) {
//            //cout << "calculate_hash FALSE2" << endl;
//            break;
//        }
//        // 计算HASH数据
//        bRet = ::CryptHashData(hCryptHash, pData, dwDataLength, 0);
//        if (FALSE == bRet) {
//            //cout << "calculate_hash FALSE3" << endl;
//            break;
//        }
//        // 获取HASH结果的大小
//        dwTemp = sizeof(dwHashDataLength);
//        bRet = ::CryptGetHashParam(hCryptHash, HP_HASHSIZE, (BYTE*)(&dwHashDataLength), &dwTemp, 0);
//        if (FALSE == bRet) {
//            //cout << "calculate_hash FALSE4" << endl;
//            break;
//        }
//        // 申请内存
//        pHashData = new BYTE[dwHashDataLength];
//        if (NULL == pHashData) {
//            bRet = FALSE;
//            //cout << "calculate_hash FALSE5" << endl;
//            break;
//        }
//        ::RtlZeroMemory(pHashData, dwHashDataLength);
//        // 获取HASH结果数据
//        bRet = ::CryptGetHashParam(hCryptHash, HP_HASHVAL, pHashData, &dwHashDataLength, 0);
//        if (FALSE == bRet) {
//            //cout << "calculate_hash FALSE6" << endl;
//            break;
//        }
//        // 返回数据
//        *ppHashData = pHashData;
//        *pdwHashDataLength = dwHashDataLength;
//    } while (FALSE);
//    // 释放关闭
//    if (FALSE == bRet) {
//        if (pHashData) {
//            delete[] pHashData;
//            pHashData = NULL;
//        }
//    }
//    if (hCryptHash) {
//        ::CryptDestroyHash(hCryptHash);
//    }
//    if (hCryptProv) {
//        ::CryptReleaseContext(hCryptProv, 0);
//    }
//    return bRet;
//}
//
//BOOL get_file_sha256_hash(const char* pszFilePath, BYTE** pHashData, DWORD* dwHashDataLength) {
//    BYTE* pData = NULL;
//    DWORD dwDataLength = 0;
//    get_file_data(pszFilePath, &pData, &dwDataLength);
//    BOOL res = calculate_hash(pData, dwDataLength, CALG_SHA_256, pHashData, dwHashDataLength);
//    delete[] pData;
//    return res;
//}
//
//EXTERN_C IMAGE_DOS_HEADER __ImageBase;
//
//string get_dll_path() {
//    TCHAR path[MAX_PATH] = { 0 };
//    GetModuleFileName((HINSTANCE)&__ImageBase, path, _countof(path));
//    return (string)path;
//}