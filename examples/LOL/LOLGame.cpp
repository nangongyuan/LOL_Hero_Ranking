#include "stdafx.h"
#include "LOLGame.h"
#include <regex>
#include <tlhelp32.h>
#include <psapi.h>
#include "spdlog/spdlog.h"
#include "MainWindow.h"

#pragma comment(lib, "Version.lib")

#define Process_Name L"LeagueClientUx.exe"

typedef LONG NTSTATUS;


LOLGame::LOLGame(MainWindow* main_window)
:FrameworkThread("GameThread")
{
	thread_id_ = kThreadGame;
	main_window_ = main_window;
}

void LOLGame::SetChampionConf()
{
	if (cur_select_champ_info_.id == 0)
		return;

	cur_champ_conf_ = opgg_request_.GetChampConf(cur_select_champ_info_.id);
	cur_champ_conf_.champ_name = all_champ_info_map_[cur_champ_conf_.champ_id].name;

	lcu_request_.SetChampionItem(cur_champ_conf_);
	lcu_request_.SetChampionPerk(cur_champ_conf_);

	spdlog::info(__FUNCTION__ " champ_info: id:{} name:{}", cur_select_champ_info_.id, cur_champ_conf_.champ_name);
}

void LOLGame::Init()
{
	nbase::ThreadManager::RegisterThread(thread_id_);

	nbase::ThreadManager::PostRepeatedTask(kThreadGame,
		nbase::Bind(&LOLGame::OnTimer, this), nbase::TimeDelta::FromMilliseconds(300), nbase::ThreadManager::TIMES_FOREVER);

	SetGameLog("初始化完成，等待游戏启动");
}


void LOLGame::Cleanup()
{
	nbase::ThreadManager::UnregisterThread();
}

LCURequest& LOLGame::GetLCURequest()
{
	return lcu_request_;
}

void LOLGame::Test()
{
	opgg_request_.Init(client_info_.version);
	all_champ_rank_info_ = opgg_request_.GetOPGGChampRankInfo();

	all_champ_info_map_ = lcu_request_.GetAllChampInfoMap();
	for (auto iter = all_champ_info_map_.begin(); iter != all_champ_info_map_.end(); iter++)
	{
		iter->second.rank = all_champ_rank_info_[iter->first].rank;
		iter->second.tier = all_champ_rank_info_[iter->first].tier;
		iter->second.win_rate = all_champ_rank_info_[iter->first].win_rate;
	}

	main_window_->SetAvailableChamp(all_champ_info_map_);

	int cur_id = lcu_request_.GetCurSelectChampionId();
	if (cur_id > 0)
	{
		cur_select_champ_info_ = GetChampInfoById(cur_id);
		SetChampionConf();
	}
}

void LOLGame::OnTimer()
{
	switch (cur_game_status_)
	{
	case GameStatus::NoLaunch:
		FindGameWindow();
		break;
	case GameStatus::Launching:
		OnClientInfo();
		break;
	case GameStatus::None:
	case GameStatus::Matchmaking:
	case GameStatus::Lobby:
	case GameStatus::ReadyCheck:
		OnGameNone();
		break;
	case GameStatus::Reconnect:
		break;
	case GameStatus::ChampSelect:
		OnChampSelect();
		break;
	case GameStatus::GameStart:
	case GameStatus::InProgress:
	case GameStatus::EndOfGame:
	case GameStatus::PreEndOfGame:
		OnGameInProgress();
	case GameStatus::WaitingForStatus:
		break;
	case GameStatus::unknown:
	{
		HWND hwnd = ::FindWindow(L"RCLIENT", L"League of Legends");
		if (hwnd == NULL)
		{
			cur_summoner_info_.summonerId = 0;
			cur_game_status_ = GameStatus::NoLaunch;
		}
		else
			cur_game_status_ = GameStatus::None;
	}
		break;
	default:
		break;
	}
}

void LOLGame::FindGameWindow()
{
	HWND hwnd = ::FindWindow(L"RCLIENT", L"League of Legends");
	if (hwnd)
	{
		SetGameStatus(GameStatus::Launching, "游戏启动中");
	}
}

void LOLGame::OnGameNone()
{
	if (!available_champ_info_map_.empty())
	{
		available_champ_info_map_.clear();
		nbase::ThreadManager::PostTask(kThreadUI,
			nbase::Bind(&MainWindow::SetAvailableChamp, main_window_, available_champ_info_map_));
	}

	std::string status;
	GameStatus game_status = lcu_request_.GetGameStatus(status);

	if (cur_game_status_ == game_status)
		return;

	SetGameStatus(game_status, status);
}

void LOLGame::OnClientInfo()
{
	DWORD pid = nbase::win32::GetProcessIDByName(Process_Name);
	spdlog::info(__FUNCTION__ " pid:{}", pid);
	if (pid == 0)
		return;

	if (cur_summoner_info_.summonerId != 0 && !all_champ_info_map_.empty()
		&& !all_champ_rank_info_.empty())
	{
		SetGameStatus(GameStatus::None, "大厅等待中");
		return;
	}

	client_info_ = GetClientInfo(pid);

	opgg_request_.Init(client_info_.version);
	lcu_request_.Init(client_info_);

	LoginInfo login_info = lcu_request_.login();

	cur_summoner_info_ = lcu_request_.GetCurSummonerInfo();

	nbase::ThreadManager::PostTask(kThreadUI,
		nbase::Bind(&MainWindow::SetSummonerInfo, main_window_, cur_summoner_info_));

	GetAllChampInfo();
}

void LOLGame::OnChampSelect()
{
	std::string status;
	GameStatus game_status = lcu_request_.GetGameStatus(status);

	if (GameStatus::ChampSelect != game_status)
	{
		SetGameStatus(game_status, status);
		return;
	}
		
	int cur_id = lcu_request_.GetCurSelectChampionId();
	if (cur_id != cur_select_champ_info_.id)
	{
		OnMyChampChanged(cur_id);
	}


	bool changed = false;
	std::vector<ChampSelectInfo> select_champs = lcu_request_.GetChampSelectInfo();
	for (auto item : select_champs)
	{
		auto iter = available_champ_info_map_.find(item.championId);
		if (iter == available_champ_info_map_.end())
		{
			available_champ_info_map_[item.championId] = GetChampInfoById(item.championId);
			changed = true;
		}
	}

	for (auto iter = available_champ_info_map_.begin(); iter != available_champ_info_map_.end(); iter++)
	{
		int id = iter->first;
		auto find_iter = std::find_if(select_champs.begin(), select_champs.end(), [id](const ChampSelectInfo& item) {
			return item.championId == id;
			});

		if (find_iter  == select_champs.end() && iter->second.otherSelected)
		{
			iter->second.otherSelected = false;
			changed = true;
		}

		if (find_iter != select_champs.end() && !iter->second.otherSelected)
		{
			iter->second.otherSelected = true;
			changed = true;
		}
	}

	if (changed)
	{
		nbase::ThreadManager::PostTask(kThreadUI,
			nbase::Bind(&MainWindow::SetAvailableChamp, main_window_, available_champ_info_map_));
	}
}

void LOLGame::OnGameInProgress()
{
	static int count = 0;
	count++;
	if (count > 5)
	{
		count = 0;
		std::string status;
		GameStatus game_status = lcu_request_.GetGameStatus(status);
		SetGameStatus(game_status, status);
	}
}

void LOLGame::OnMyChampChanged(int id)
{
	ChampInfo champ_info = GetChampInfoById(id);
	SetGameLog(std::string("选中英雄：") + champ_info.name);

	cur_select_champ_info_ = champ_info;

	nbase::ThreadManager::PostTask(kThreadUI,
		nbase::Bind(&MainWindow::SetChampInfo, main_window_, champ_info));
}

void LOLGame::SetGameLog(const std::string& log)
{
	nbase::ThreadManager::PostTask(kThreadUI,
		nbase::Bind(&MainWindow::SetGameLog, main_window_, log));
}

void LOLGame::SetGameStatus(GameStatus game_status, std::string status)
{
	if (cur_game_status_ == game_status)
		return;

	cur_game_status_ = game_status;
	SetGameLog(std::string("游戏状态变化：") + status);
	spdlog::info(__FUNCTION__ " status:{}", status);
}

ChampInfo LOLGame::GetChampInfoById(int id)
{
	return all_champ_info_map_[id];
}

void LOLGame::GetAllChampInfo()
{
	spdlog::info(__FUNCTION__);

	if (all_champ_info_map_.empty())
	{
		all_champ_info_map_ = lcu_request_.GetAllChampInfoMap();
	}

	if (all_champ_info_map_.empty()) return;

	if (all_champ_rank_info_.empty())
	{
		all_champ_rank_info_ = opgg_request_.GetOPGGChampRankInfo();

		for (auto iter = all_champ_rank_info_.begin(); iter != all_champ_rank_info_.end(); iter++)
		{
			all_champ_info_map_[iter->first].rank = iter->second.rank;
			all_champ_info_map_[iter->first].tier = iter->second.tier;
			all_champ_info_map_[iter->first].win_rate = iter->second.win_rate;

			lcu_request_.DownloadImage(all_champ_info_map_[iter->first].squarePortraitPath);
		}
	}
	
	
}

std::wstring LOLGame::GetProcessCommandLine(const DWORD& processId)
{
	using tNtQueryInformationProcess = NTSTATUS(__stdcall*)
		(
			HANDLE ProcessHandle,
			ULONG ProcessInformationClass,
			PVOID ProcessInformation,
			ULONG ProcessInformationLength,
			PULONG ReturnLength
			);

	static HMODULE kernel32 = GetModuleHandleA("kernel32");

	static auto pOpenProcess = (decltype(&OpenProcess))GetProcAddress(kernel32, "OpenProcess");
	std::wstring result;
	const HANDLE processHandle = pOpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, processId);

	static auto pGetNativeSystemInfo = (decltype(&GetNativeSystemInfo))GetProcAddress(kernel32, "GetNativeSystemInfo");
	SYSTEM_INFO si;
	pGetNativeSystemInfo(&si);

	static auto pIsWow64Process = (decltype(&IsWow64Process))GetProcAddress(kernel32, "IsWow64Process");
	static auto pGetCurrentProcess = (decltype(&GetCurrentProcess))GetProcAddress(kernel32, "GetCurrentProcess");
	BOOL wow;
	pIsWow64Process(pGetCurrentProcess(), &wow);

	const DWORD ProcessParametersOffset = si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ? 0x20 : 0x10;
	const DWORD CommandLineOffset = si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ? 0x70 : 0x40;

	const DWORD pebSize = ProcessParametersOffset + 8; // size until ProcessParameters
	const auto peb = static_cast<PBYTE>(malloc(pebSize));
	ZeroMemory(peb, pebSize);

	const DWORD processParametersSize = CommandLineOffset + 16;
	const auto processParameters = static_cast<PBYTE>(malloc(processParametersSize));
	ZeroMemory(processParameters, processParametersSize);

	if (wow)
	{
		using PROCESS_BASIC_INFORMATION_WOW64 = struct _PROCESS_BASIC_INFORMATION_WOW64
		{
			PVOID Reserved1[2];
			PVOID64 PebBaseAddress;
			PVOID Reserved2[4];
			ULONG_PTR UniqueProcessId[2];
			PVOID Reserved3[2];
		};

		using UNICODE_STRING_WOW64 = struct _UNICODE_STRING_WOW64
		{
			USHORT Length;
			USHORT MaximumLength;
			PVOID64 Buffer;
		};

		using tNtWow64ReadVirtualMemory64 = NTSTATUS(NTAPI*)(
			IN HANDLE ProcessHandle,
			IN PVOID64 BaseAddress,
			OUT PVOID Buffer,
			IN ULONG64 Size,
			OUT PULONG64 NumberOfBytesRead);

		PROCESS_BASIC_INFORMATION_WOW64 pbi;
		ZeroMemory(&pbi, sizeof(pbi));

		const auto NtQueryInformationProcess =
			reinterpret_cast<tNtQueryInformationProcess>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWow64QueryInformationProcess64"));
		auto status = NtQueryInformationProcess(processHandle, 0, &pbi, sizeof(pbi), nullptr);
		if (status != 0)
		{
			spdlog::error("NtWow64QueryInformationProcess64 failed, error code: {}", status);
			CloseHandle(processHandle);
			return {};
		}

		const auto NtWow64ReadVirtualMemory64 =
			reinterpret_cast<tNtWow64ReadVirtualMemory64>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWow64ReadVirtualMemory64"));

		status = NtWow64ReadVirtualMemory64(processHandle, pbi.PebBaseAddress, peb, pebSize, nullptr);
		if (status != 0)
		{
			spdlog::error("PEB NtWow64ReadVirtualMemory64 failed, error code: {}", status);
			CloseHandle(processHandle);
			return {};
		}

		const auto parameters = *reinterpret_cast<PVOID64*>(peb + ProcessParametersOffset);
		status = NtWow64ReadVirtualMemory64(
			processHandle, parameters, processParameters, processParametersSize, nullptr);
		if (status != 0)
		{
			spdlog::error("processParameters NtWow64ReadVirtualMemory64 failed, error code: {}", status);
			CloseHandle(processHandle);
			return {};
		}

		const UNICODE_STRING_WOW64* pCommandLine = reinterpret_cast<UNICODE_STRING_WOW64*>(processParameters + CommandLineOffset);
		const auto commandLineCopy = static_cast<PWSTR>(malloc(pCommandLine->MaximumLength));
		status = NtWow64ReadVirtualMemory64(processHandle, pCommandLine->Buffer, commandLineCopy, pCommandLine->MaximumLength, nullptr);
		if (status != 0)
		{
			spdlog::error("pCommandLine NtWow64ReadVirtualMemory64 failed, error code: {}", status);
			CloseHandle(processHandle);
			return {};
		}

		result = std::wstring(commandLineCopy);
		CloseHandle(processHandle);
	}
	else
	{
		using PROCESS_BASIC_INFORMATION = struct _PROCESS_BASIC_INFORMATION
		{
			LONG ExitStatus;
			PVOID PebBaseAddress;
			ULONG_PTR AffinityMask;
			LONG BasePriority;
			HANDLE UniqueProcessId;
			HANDLE InheritedFromUniqueProcessId;
		};

		typedef struct _UNICODE_STRING
		{
			USHORT Length;
			USHORT MaximumLength;
			PWSTR Buffer;
		} UNICODE_STRING, * PUNICODE_STRING [[maybe_unused]];
		/*[[maybe_unused]]*/ using PCUNICODE_STRING = const UNICODE_STRING*;

		PROCESS_BASIC_INFORMATION pbi;
		ZeroMemory(&pbi, sizeof(pbi));

		const auto NtQueryInformationProcess =
			reinterpret_cast<tNtQueryInformationProcess>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess"));
		auto status = NtQueryInformationProcess(processHandle, 0, &pbi, sizeof(pbi), nullptr);
		if (status != 0)
		{
			spdlog::error("NtQueryInformationProcess failed, error code: {}", status);
			CloseHandle(processHandle);
			return {};
		}

		static auto pReadProcessMemory = (decltype(&ReadProcessMemory))GetProcAddress(kernel32, "ReadProcessMemory");
		if (!pReadProcessMemory(processHandle, pbi.PebBaseAddress, peb, pebSize, nullptr))
		{
			spdlog::error("PEB ReadProcessMemory failed, error code: {}", GetLastError());
			CloseHandle(processHandle);
			return {};
		}
		const PBYTE* parameters = static_cast<PBYTE*>(*reinterpret_cast<LPVOID*>(peb + ProcessParametersOffset));
		if (!pReadProcessMemory(
			processHandle, parameters, processParameters, processParametersSize, nullptr))
		{
			spdlog::error("processParameters ReadProcessMemory failed, error code: {}", GetLastError());
			CloseHandle(processHandle);
			return {};
		}

		const UNICODE_STRING* pCommandLine = reinterpret_cast<UNICODE_STRING*>(processParameters + CommandLineOffset);
		const auto commandLineCopy = static_cast<PWSTR>(malloc(pCommandLine->MaximumLength));
		if (!pReadProcessMemory(processHandle, pCommandLine->Buffer, commandLineCopy, pCommandLine->MaximumLength, nullptr))
		{
			spdlog::error("pCommandLine ReadProcessMemory failed, error code: {}", GetLastError());
			CloseHandle(processHandle);
			return {};
		}

		result = std::wstring(commandLineCopy);
		CloseHandle(processHandle);
	}

	return result;
}

ClientInfo LOLGame::GetClientInfo(const DWORD& pid, bool riotClient)
{
	if (!pid)
		return {};
	const std::string cmdLine = nbase::UTF16ToUTF8(GetProcessCommandLine(pid));
	if (cmdLine.empty())
		return {};

	ClientInfo info;
	info.port = GetPort(cmdLine, riotClient);
	info.token = GetToken(cmdLine, riotClient);
	info.path = GetProcessPath(pid);
	info.version = GetFileVersion(info.path);

	spdlog::info(__FUNCTION__ " port:{} path:{} version:{}", info.port, nbase::UTF16ToUTF8(info.path), info.version);
	return info;
}

int LOLGame::GetPort(const std::string& cmdLine, bool riotClient)
{
	std::regex regexStr;
	regexStr = riotClient ? "--riotclient-app-port=(\\d*)" : "--app-port=(\\d*)";
	std::smatch m;
	if (std::regex_search(cmdLine, m, regexStr))
		return std::stoi(m[1].str());

	return 0;
}

std::string LOLGame::GetToken(const std::string& cmdLine, bool riotClient)
{
	std::regex regexStr;
	regexStr = riotClient ? "--riotclient-auth-token=([\\w-]*)" : "--remoting-auth-token=([\\w-]*)";
	std::smatch m;
	if (std::regex_search(cmdLine, m, regexStr))
	{
		std::string token = "riot:" + m[1].str();
		std::string result;

		if (nbase::Base64Encode(token, &result))
			return result;
	}

	return "";
}

std::wstring LOLGame::GetProcessPath(const DWORD& processId)
{
	static HMODULE kernel32 = GetModuleHandleA("kernel32");
	static auto pOpenProcess = (decltype(&OpenProcess))GetProcAddress(kernel32, "OpenProcess");

	if (const HANDLE processHandle = pOpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, processId))
	{
		static auto pK32GetModuleFileNameExW = (decltype(&K32GetModuleFileNameExW))GetProcAddress(kernel32, "K32GetModuleFileNameExW");
		WCHAR result[MAX_PATH];
		if (pK32GetModuleFileNameExW(processHandle, nullptr, result, MAX_PATH))
		{
			CloseHandle(processHandle);
			
			return result;
		}
		CloseHandle(processHandle);
	}
	return L"";
}

std::string LOLGame::GetFileVersion(const std::wstring& file)
{
	if (const DWORD versionSize = GetFileVersionInfoSizeW(file.c_str(), nullptr))
	{
		std::vector<unsigned char> versionInfo(versionSize);

		if (GetFileVersionInfoW(file.c_str(), 0, versionSize, versionInfo.data()))
		{
			VS_FIXEDFILEINFO* lpFfi = nullptr;
			UINT size = sizeof(VS_FIXEDFILEINFO);
			if (VerQueryValueW(versionInfo.data(), L"\\", reinterpret_cast<LPVOID*>(&lpFfi), &size))
			{
				const DWORD dwFileVersionMS = lpFfi->dwFileVersionMS;
				const DWORD dwFileVersionLS = lpFfi->dwFileVersionLS;
				std::string result = nbase::StringPrintf("%d.%d",
					HIWORD(dwFileVersionMS), LOWORD(dwFileVersionMS));
				return result;
			}
		}
	}

	return "";
}
