#include <iostream>
#include <chrono>
#include <string>
#include <windows.h>
#include <tlHelp32.h>
#include "OneLine.h"
#include "psapi.h"
#include "StringUtils.h"

using namespace std;

long long get_time() {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}

void get_process() {
	PROCESSENTRY32 pe32;
	//在使用这个结构之前，先设置它的大小
	pe32.dwSize = sizeof(pe32);
	//给系统内所有的进程拍一个快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		printf("CreateToolhelp32Snapshot调用失败\n");
		return;
	}
	//遍历进程快照，轮流显示每个进程的信息
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore) {
		if (strcmp("FlashHelper.exe", pe32.szExeFile) == 0) {
			printf("进程名称：%ls\n", pe32.szExeFile);
			printf("进程ID号：%u\n\n", pe32.th32ProcessID);
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	//不要忘记清除snapshot对象
	::CloseHandle(hProcessSnap);
}

void enum_process() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    size_t i;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return;
    }
    cProcesses = cbNeeded / sizeof(DWORD);
    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            DWORD pid = aProcesses[i];
            TCHAR szProcessName[MAX_PATH] = TEXT("???");
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
            if (NULL != hProcess)
            {
                HMODULE hMod;
                DWORD cbNeeded;
                /*if (EnumProcessodules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                {
                    GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
                }*/
            }
            printf("PID: %u\t%ls\n", pid, szProcessName);
            CloseHandle(hProcess);
        }
    }
}

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

void check_process() {
    TCHAR DllPath[512] = { 0 };
    GetModuleFileName((HINSTANCE)&__ImageBase, DllPath, _countof(DllPath));
    string str = (string)DllPath;
    cout << "str:" + str << endl;
}

void ShowError(const char* pszText)
{
    char szErr[MAX_PATH] = { 0 };
    ::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
#ifdef _DEBUG
    ::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}

BOOL GetFileData(const char* pszFilePath, BYTE** ppFileData, DWORD* pdwFileDataLength)
{
    BOOL bRet = TRUE;
    BYTE* pFileData = NULL;
    DWORD dwFileDataLength = 0;
    HANDLE hFile = NULL;
    DWORD dwTemp = 0;

    do
    {
        hFile = ::CreateFile(pszFilePath, GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_ARCHIVE, NULL);
        if (INVALID_HANDLE_VALUE == hFile)
        {
            bRet = FALSE;
            ShowError("CreateFile");
            break;
        }

        dwFileDataLength = ::GetFileSize(hFile, NULL);

        pFileData = new BYTE[dwFileDataLength];
        if (NULL == pFileData)
        {
            bRet = FALSE;
            ShowError("new");
            break;
        }
        ::RtlZeroMemory(pFileData, dwFileDataLength);

        ::ReadFile(hFile, pFileData, dwFileDataLength, &dwTemp, NULL);

        // 返回
        *ppFileData = pFileData;
        *pdwFileDataLength = dwFileDataLength;

    } while (FALSE);

    if (hFile)
    {
        ::CloseHandle(hFile);
    }

    return bRet;
}

BOOL CalculateHash(BYTE* pData, DWORD dwDataLength, ALG_ID algHashType, BYTE** ppHashData, DWORD* pdwHashDataLength)
{
    HCRYPTPROV hCryptProv = NULL;
    HCRYPTHASH hCryptHash = NULL;
    BYTE* pHashData = NULL;
    DWORD dwHashDataLength = 0;
    DWORD dwTemp = 0;
    BOOL bRet = FALSE;

    do
    {
        // 获得指定CSP的密钥容器的句柄
        bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
        if (FALSE == bRet)
        {
            ShowError("CryptAcquireContext");
            break;
        }

        // 创建一个HASH对象, 指定HASH算法
        bRet = ::CryptCreateHash(hCryptProv, algHashType, NULL, NULL, &hCryptHash);
        if (FALSE == bRet)
        {
            ShowError("CryptCreateHash");
            break;
        }

        // 计算HASH数据
        bRet = ::CryptHashData(hCryptHash, pData, dwDataLength, 0);
        if (FALSE == bRet)
        {
            ShowError("CryptHashData");
            break;
        }

        // 获取HASH结果的大小
        dwTemp = sizeof(dwHashDataLength);
        bRet = ::CryptGetHashParam(hCryptHash, HP_HASHSIZE, (BYTE*)(&dwHashDataLength), &dwTemp, 0);
        if (FALSE == bRet)
        {
            ShowError("CryptGetHashParam");
            break;
        }

        // 申请内存
        pHashData = new BYTE[dwHashDataLength];
        if (NULL == pHashData)
        {
            bRet = FALSE;
            ShowError("new");
            break;
        }
        ::RtlZeroMemory(pHashData, dwHashDataLength);

        // 获取HASH结果数据
        bRet = ::CryptGetHashParam(hCryptHash, HP_HASHVAL, pHashData, &dwHashDataLength, 0);
        if (FALSE == bRet)
        {
            ShowError("CryptGetHashParam");
            break;
        }

        // 返回数据
        *ppHashData = pHashData;
        *pdwHashDataLength = dwHashDataLength;

    } while (FALSE);

    // 释放关闭
    if (FALSE == bRet)
    {
        if (pHashData)
        {
            delete[]pHashData;
            pHashData = NULL;
        }
    }
    if (hCryptHash)
    {
        ::CryptDestroyHash(hCryptHash);
    }
    if (hCryptProv)
    {
        ::CryptReleaseContext(hCryptProv, 0);
    }

    return bRet;
}

int main() {
    /*BYTE* pData = NULL;
    DWORD dwDataLength = 0;
    DWORD i = 0;
    BYTE* pHashData = NULL;
    DWORD dwHashDataLength = 0;
    GetFileData("C:\\Projects\\C#\\bxxt\\bxxt_client\\FlashHelper\\bin\\x64\\Release\\FlashHelper.exe", &pData, &dwDataLength);
    CalculateHash(pData, dwDataLength, CALG_SHA_256, &pHashData, &dwHashDataLength);
    printf("SHA256[%ld]\n", dwHashDataLength);
    for (i = 0; i < dwHashDataLength; i++)
    {
        printf("%X", pHashData[i]);
    }
    printf("\n\n");

    check_process();*/
    long long startTime = get_time();
    std::cout << std::to_string(startTime) << std::endl;
    char var1[] = "2,6b,40,20,0,4,1";
    char* var2 = new char[2048];
    one_line_main(var1, var2);
    std::cout << std::to_string(get_time() - startTime) << std::endl;
    system("pause");
}

