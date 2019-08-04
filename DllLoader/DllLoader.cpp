#include <iostream>
#include <Windows.h>
#include <Tlhelp32.h>
#include <chrono>
#include <thread>

#include <direct.h>
#define GetCurrentDir _getcwd

#define WECHAT_PROCESS_NAME "WeChat.exe"
#define Dll_NAME_2B_INJECTED "WeChook.dll"

// 1. Get PID of WeChat.
DWORD GetPidByProcessName(LPCSTR ProcessName) {
	// get a snapshot of all the process in the system.
	HANDLE ProcessAll = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	// compare the process name in the snapshot to find pid
	PROCESSENTRY32 processInfo = { 0 };
	processInfo.dwSize = sizeof(PROCESSENTRY32);
	while (Process32Next(ProcessAll, &processInfo)) {
		if (strcmp(ProcessName, processInfo.szExeFile) == 0) {
			return processInfo.th32ProcessID;
		}
	}
	return 0;
}

// 2. init a memory block in wechat to store the path to our dll, 
VOID InjectDll() {
	CHAR pathStr[0x200] = { 0 };
	CHAR dllName[] = Dll_NAME_2B_INJECTED;
	if (!GetCurrentDir(pathStr, sizeof(pathStr)))
	{
		std::cout << "Can not get the current dir.\n";
		return;
	}
	pathStr[sizeof(pathStr) - 1] = '\0';
	strcat_s(pathStr, "\\");
	strcat_s(pathStr, dllName);
	//printf("%s\n", pathStr);


	// get WeChat.exe's process handler 
	DWORD PID = GetPidByProcessName(WECHAT_PROCESS_NAME);
	if (PID == 0) {
		std::cout << "Did not find the PID of " << WECHAT_PROCESS_NAME << "\n";
		return;
	}
	std::cout << "PID of " << WECHAT_PROCESS_NAME << " is: " << PID << "\n";
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (hProcess == NULL) {
		std::cout << "Did not get the process handler of " << WECHAT_PROCESS_NAME << "\n";
		return;
	}

	// init memory block
	LPVOID dllPathAddr = VirtualAllocEx(hProcess, NULL, strlen(pathStr), MEM_COMMIT, PAGE_READWRITE);
	if (dllPathAddr == NULL) {
		std::cout << "Failed to init memory block.\n";
		return;
	}

	// write the path of dll to this memory block
	if (WriteProcessMemory(hProcess, dllPathAddr, pathStr, strlen(pathStr), NULL) == 0) {
		std::cout << "Failed to inject the path of dll to the memory block\n";
		return;
	}

	// get the address of LoadLibrary() in kernel32.dll
	HMODULE k32 = GetModuleHandle("Kernel32.dll");
	LPVOID loadAddr = GetProcAddress(k32, "LoadLibraryA");
	// execute the dll as a remote thread
	HANDLE newThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadAddr, dllPathAddr, 0, NULL);
	if (newThread == NULL) {
		std::cout << "Failed to create a remote thread.\n";
	}
	std::cout << "Dll injected.\n";
	WaitForSingleObject(newThread, INFINITE);
	CloseHandle(newThread);
	CloseHandle(hProcess);
}

VOID EjectDll() {
	char szDllName[] = Dll_NAME_2B_INJECTED;
	DWORD PID = GetPidByProcessName(WECHAT_PROCESS_NAME);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	MODULEENTRY32 ME32 = { 0 };
	ME32.dwSize = sizeof(MODULEENTRY32);
	BOOL isNext = Module32First(hSnap, &ME32);
	BOOL flag = FALSE;
	while (isNext) {
		if (strcmp(ME32.szModule, szDllName) == 0) {
			flag = TRUE;
			break;
		}
		isNext = Module32Next(hSnap, &ME32);
	}
	if (flag == FALSE) {
		std::cout << "Can not find the dll module.\n";
		return;
	}
	CloseHandle(hSnap);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	FARPROC pFun = GetProcAddress(GetModuleHandle("kernel32.dll"), "FreeLibrary");
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFun, ME32.modBaseAddr, 0, NULL);
	if (hThread == NULL) {
		std::cout << "Failed to create a remote thread.\n";
	}
	std::cout << "Dll ejected.\n";
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
}

int main()
{
	InjectDll();
	system("pause");
	EjectDll();
	return 0;
}