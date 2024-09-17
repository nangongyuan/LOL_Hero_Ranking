// controls.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "main.h"
#include "MainWindow.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <dbghelp.h>

#pragma comment(lib, "Dbghelp.lib")

LONG WINAPI MyExceptionFilter(EXCEPTION_POINTERS* pExceptionPointers) 
{
	spdlog::flush_every(std::chrono::seconds(0));

	HANDLE hFile = CreateFile(nbase::StringPrintf(L"fulldump_%d.dmp", ::GetCurrentProcessId()).c_str(), 
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ExceptionPointers = pExceptionPointers;
		dumpInfo.ClientPointers = TRUE;

		// 使用 MiniDumpWithFullMemory 生成完整的 Dump 文件
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &dumpInfo, NULL, NULL);
		CloseHandle(hFile);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	SetUnhandledExceptionFilter(MyExceptionFilter);

	std::wstring log_path = nbase::win32::GetCurrentModuleDirectory() + L"/logs/LOG.txt";
	// Set the default logger to file logger
	auto file_logger = spdlog::basic_logger_mt("basic_logger", nbase::UTF16ToUTF8(log_path));
	spdlog::set_default_logger(file_logger);
	spdlog::flush_every(std::chrono::seconds(3));


	MainThread().RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

	return 0;
}

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);

	// 获取资源路径，初始化全局参数
	std::wstring app_dir = nbase::win32::GetCurrentModuleDirectory();
	ui::GlobalManager::Startup(app_dir + L"resources\\", ui::CreateControlCallback(), false);

	ui::GlobalManager::EnableAutomation();

	// 创建一个默认带有阴影的居中窗口
	MainWindow* window = new MainWindow();
	window->Create(NULL, MainWindow::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	window->CenterWindow();
	window->ShowWindow();
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();

	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}
