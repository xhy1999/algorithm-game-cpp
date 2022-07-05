#include <iostream>
#include <chrono>
#include <string>
#include <windows.h>
#include <tlHelp32.h>
#include "OneLine.h"
#include "psapi.h"
#include "StringUtils.h"
#include "FileUtils.h"

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

int main() {
    //BYTE* pData = NULL;
    //DWORD dwDataLength = 0;
    //DWORD i = 0;
    BYTE* pHashData = NULL;
    DWORD dwHashDataLength = 0;
    get_file_sha256_hash("C:\\Projects\\C#\\bxxt\\bxxt_client\\FlashHelper\\bin\\x64\\Release\\FlashHelper.exe", &pHashData, &dwHashDataLength);
    //GetFileData("C:\\Projects\\C#\\bxxt\\bxxt_client\\FlashHelper\\bin\\x64\\Release\\FlashHelper.exe", &pData, &dwDataLength);
    //CalculateHash(pData, dwDataLength, CALG_SHA_256, &pHashData, &dwHashDataLength);
    //printf("SHA256[%ld]\n", dwHashDataLength);
    for (int i = 0; i < dwHashDataLength; i++) {
        printf("%X", pHashData[i]);
        //cout << to_string((int)pHashData[i])<<endl;
    }
    cout << to_string((int)pHashData[5]) << endl;
    cout << to_string((int)pHashData[6]) << endl;
    cout << to_string((int)pHashData[7]) << endl;
    cout << to_string((int)pHashData[8]) << endl;
    cout << to_string((int)pHashData[9]) << endl;
    cout << to_string((int)pHashData[dwHashDataLength - 10]) << endl;
    cout << to_string((int)pHashData[dwHashDataLength - 9]) << endl;
    cout << to_string((int)pHashData[dwHashDataLength - 8]) << endl;
    cout << to_string((int)pHashData[dwHashDataLength - 7]) << endl;
    cout << to_string((int)pHashData[dwHashDataLength - 6]) << endl;
    
    printf("\n\n");
    string_2_char("123456");

    system("pause");
    long long startTime = get_time();
    std::cout << std::to_string(startTime) << std::endl;
    char var1[] = "2,6b,40,20,0,4,1";
    char* var2 = new char[2048];
    one_line_main(var1, var2);
    std::cout << std::to_string(get_time() - startTime) << std::endl;
    system("pause");
}

