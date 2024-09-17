#include "Utils.h"
#include <psapi.h>
#include <intrin.h>
#include <TlHelp32.h>
#include <winternl.h>
#include <string>

extern HWND hServerWnd;
extern HANDLE g_hProcess;;
namespace Utils {
	BOOLEAN MaskCompare(PVOID buffer, LPCSTR pattern, LPCSTR mask) {
		for (auto b = reinterpret_cast<PBYTE>(buffer); *mask; ++pattern, ++mask, ++b) {
			if (*mask == 'x' && *reinterpret_cast<LPCBYTE>(pattern) != *b) {
				return FALSE;
			}
		}

		return TRUE;
	}
	PBYTE FindPattern(PVOID base, DWORD size, LPCSTR pattern, LPCSTR mask) {
		size -= static_cast<DWORD>(strlen(mask));

		for (auto i = 0UL; i < size; ++i) {
			auto addr = reinterpret_cast<PBYTE>(base) + i;
			if (MaskCompare(addr, pattern, mask)) {
				return addr;
			}
		}

		return NULL;
	}
	PBYTE FindPattern(LPCSTR pattern, LPCSTR mask, HMODULE mod) {
		MODULEINFO info = { 0 };
		if (mod == NULL) {
			mod = GetModuleHandle(0);
		}

		GetModuleInformation(GetCurrentProcess(), mod, &info, sizeof(info));

		return FindPattern(info.lpBaseOfDll, info.SizeOfImage, pattern, mask);
	}

	BOOL InstallSafeThread(
		__in LPVOID function
	) {

		HMODULE hModule = GetModuleHandleA(0);

		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, GetCurrentProcessId());
		if (hSnapshot == INVALID_HANDLE_VALUE)
			return FALSE;
		THREADENTRY32 te32 = { 0,0,0,0,0,0,0 };
		te32.dwSize = sizeof(THREADENTRY32);
		if (!Thread32First(hSnapshot, &te32))
		{
			CloseHandle(hSnapshot);
			return FALSE;
		}
		do
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, false, te32.th32ThreadID);
			if (hThread)
			{
				DWORD_PTR dwStartAddress = 0;
				if (NT_SUCCESS(NtQueryInformationThread(hThread, static_cast<THREADINFOCLASS>(9), &dwStartAddress, sizeof(DWORD_PTR), NULL)))
				{
					PIMAGE_DOS_HEADER DosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hModule);
					PIMAGE_NT_HEADERS NtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>((reinterpret_cast<DWORD_PTR>(hModule) + DosHeader->e_lfanew));
					if (!NtHeader || NtHeader->Signature != IMAGE_NT_SIGNATURE)
					{
						SetLastError(1793);
						return FALSE;
					}
					if (dwStartAddress > reinterpret_cast<DWORD_PTR>(hModule) &&
						dwStartAddress < (reinterpret_cast<DWORD_PTR>(hModule + 0x1000) + NtHeader->OptionalHeader.SizeOfImage))
					{
						//MessageBoxA(0, 0, 0, 0);
						_MEMORY_BASIC_INFORMATION mbi;
						CONTEXT ctx;
						DWORD Old = 0;
						ZeroMemory(&mbi, sizeof(mbi));
						if (!VirtualQuery(reinterpret_cast<LPCVOID>(dwStartAddress), &mbi, sizeof(mbi)))
						{
							CloseHandle(hSnapshot);
							CloseHandle(hThread);
							return FALSE;
						}
						if (!VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &Old))
						{
							CloseHandle(hSnapshot);
							CloseHandle(hThread);
							return FALSE;

						}
						HANDLE Thread = CreateRemoteThread(GetCurrentProcess(), 0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(dwStartAddress), 0, CREATE_SUSPENDED, 0);
						if (Thread == 0 || Thread == INVALID_HANDLE_VALUE)
						{
							TerminateThread(Thread, 0);
							CloseHandle(Thread);
							CloseHandle(hSnapshot);
							CloseHandle(hThread);
							return FALSE;
						}
						ctx.ContextFlags = CONTEXT_ALL;
						if (!GetThreadContext(Thread, &ctx))
						{
							TerminateThread(Thread, 0);
							CloseHandle(Thread);
							CloseHandle(hSnapshot);
							CloseHandle(hThread);
							return FALSE;
						}
#ifdef _WIN64
						ctx.Rcx = reinterpret_cast<DWORD64>(function);
#else
						ctx.Eax = reinterpret_cast<DWORD>(function);
#endif

						if (!SetThreadContext(Thread, &ctx))
						{
							TerminateThread(Thread, 0);
							CloseHandle(Thread);
							CloseHandle(hSnapshot);
							CloseHandle(hThread);
							return FALSE;
						}
						ResumeThread(Thread);
						CloseHandle(Thread);
						CloseHandle(hSnapshot);
						CloseHandle(hThread);
						return TRUE;
					}

				}
				CloseHandle(hThread);
			}
		} while (Thread32Next(hSnapshot, &te32));
		CloseHandle(hSnapshot);
		return FALSE;

	}


	bool is_existing_file(const wchar_t* path)
	{
		HANDLE hfile(NULL); // 文件句柄

		// 重新设置错误代码，避免发生意外
		::SetLastError(ERROR_SUCCESS);

		// 直接打开文件
		hfile = ::CreateFileW //
		(
			path,
			FILE_READ_EA,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING, // 打开一个存在的文件
			0,
			NULL //
		);

		DWORD error = ::GetLastError();

		if (hfile == NULL || hfile == INVALID_HANDLE_VALUE)
		{
			// 打开文件失败，检查错误代码
			// 注意：有时候即使文件存在，也可能会打开失败，如拒绝访问的情况
			return error != ERROR_PATH_NOT_FOUND &&
				error != ERROR_FILE_NOT_FOUND;
		}
		else
		{
			// 打开成功，文件存在

			// 记得关闭句柄释放资源
			::CloseHandle(hfile);
			hfile = NULL;

			return true;
		}
	}


	INT64 SearchByte(BYTE* 字节数组, INT64 字节个数) {
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);//0x1 4039 4900
		/*INT64 起始地址 = 0x0000000000400000;
		INT64 结束地址 = 0x7ffeffffff;*/
		INT64 起始地址 = (DWORD64)GetModuleHandle(L"crossfire.exe"); //0x140000000;
		INT64 结束地址 = 0x7ffeffffff;
		INT64 i, j;
		MEMORY_BASIC_INFORMATION mbInfo = { 0 };
		const SIZE_T BaseInfoLen = sizeof(MEMORY_BASIC_INFORMATION);
		BYTE* lpBuf = new BYTE[1];
		DWORD dwBufSize = 1;
		for (i = 起始地址; i < 结束地址;) {
			VirtualQuery((LPVOID)i, &mbInfo, BaseInfoLen);
			if (mbInfo.Type != MEM_MAPPED && mbInfo.Protect != 16 && mbInfo.Protect != 1 && mbInfo.Protect != 128) {
				delete[] lpBuf;
				dwBufSize = mbInfo.RegionSize;
				lpBuf = new BYTE[dwBufSize];
				if (ReadProcessMemory((HANDLE)-1, (LPCVOID)i, lpBuf, dwBufSize, 0)) {
					for (j = 0; j <= ((INT64)mbInfo.RegionSize - 字节个数); j++) {
						for (INT64 k = 0; k < 字节个数; k++) {
							if (lpBuf[j + k] != 字节数组[k] && 字节数组[k] != 0x99) {
								goto s;
							}
						}
						if ((i + j) != (INT64)字节数组) {
							return i + j;
						}
					s:;
					}
				}
			}
			i = (INT64)mbInfo.BaseAddress + mbInfo.RegionSize;
		}
		delete[] lpBuf;
		return 0;
	}

	BOOL InjectDll(HWND hServerWnd, LPSTR szDllPath)
	{

		HANDLE  hThread = NULL;

		HMODULE hMod = NULL;

		LPVOID pRemoteBuf = NULL;

		DWORD dwBufSize = (DWORD)(strlen(szDllPath) + 1);

		LPTHREAD_START_ROUTINE pThreadProc;


		//取得进程句柄

		//分配内存

		pRemoteBuf = VirtualAllocEx(hServerWnd, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);

		//把DLL地址写入内存

		WriteProcessMemory(hServerWnd, pRemoteBuf, (LPVOID)szDllPath, dwBufSize, NULL);

		//获取LoadLibrary具体位置

		//pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
		pThreadProc = (LPTHREAD_START_ROUTINE)0x75DF0590;
		//创建远程线程，实现DLL注入

		hThread = CreateRemoteThread(hServerWnd, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);

		WaitForSingleObject(hThread, INFINITE);

		CloseHandle(hThread);

		return TRUE;
	}

	DWORD64 ProcessModulesBase(DWORD  dwProcessId)
	{
		HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
		MODULEENTRY32 me32;


		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);


		//  Set the size of the structure before using it. 
		me32.dwSize = sizeof(MODULEENTRY32);

		if (!Module32First(hModuleSnap, &me32))
		{
			CloseHandle(hModuleSnap);    // Must clean up the
			//   snapshot object! 
			return 0;
		}

		//  Now walk the module list of the process, 
		//  and display information about each module 
		do
		{
			//printf("\n\n     MODULE NAME:     %s",
			//	me32.szModule);
			//printf("\n     executable     = %s",
			//	me32.szExePath);
			//printf("\n     process ID     = 0x%08X",
			//	me32.th32ProcessID);
			//printf("\n     ref count (g)  =     0x%04X",
			//	me32.GlblcntUsage);
			//printf("\n     ref count (p)  =     0x%04X",
			//	me32.ProccntUsage);
			//printf("\n     base address   = 0x%08X",
			//	(DWORD)me32.modBaseAddr);
			//printf("\n     base size      = %d",
			//	me32.modBaseSize);
			//if (905216 == me32.modBaseSize)    //909312
			//{
			//	return (int)me32.modBaseAddr;
			//}

			LOG(INFO) << LOG_VAR(me32.szModule);

			std::wstring strModName = me32.szModule;
			if (strModName == TEXT("netutils.dll") || strModName == TEXT("NETUTILS.DLL"))
			{
				CloseHandle(hModuleSnap);
				return (DWORD64)me32.modBaseAddr;
			}
		} while (Module32Next(hModuleSnap, &me32));


		CloseHandle(hModuleSnap);
		return 0;
	}

	DWORD64 remoteInit()
	{
		DWORD dwProcessId;
		do
		{
			hServerWnd = FindWindowA("TWINCONTROL", "WeGame");
			Sleep(1000);
		} while (hServerWnd == 0);
		::GetWindowThreadProcessId(hServerWnd, &dwProcessId);

		g_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

		DWORD64 base = ProcessModulesBase(dwProcessId);

		if (base == 0)
			return 0;


		return base + 0x14290;
	}

	void AdjustPrivilege()
	{
		HANDLE hToken;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
		{
			TOKEN_PRIVILEGES tp;
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
			{
				AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
			}
			CloseHandle(hToken);
		}
	}
}
