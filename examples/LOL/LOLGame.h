#pragma once

#include "Common.h"
#include "LCURequest.h"
#include "OPGGRequest.h"

class MainWindow;

class Game : public nbase::FrameworkThread
{
public:
	Game(MainWindow* main_window);

	void SetChampionConf();

	LCURequest& GetLCURequest();

	void Test();

private:
	/**
	* 虚函数，初始化线程
	* @return void	无返回值
	*/
	virtual void Init() override;

	/**
	* 虚函数，线程退出时，做一些清理工作
	* @return void	无返回值
	*/
	virtual void Cleanup() override;

	enum ThreadId thread_id_;

private:

	void OnTimer();

	void FindGameWindow();

	void OnGameNone();

	void OnClientInfo();

	void OnChampSelect();
  
	void OnGameInProgress();

	void OnMyChampChanged(int id);

	void SetGameLog(const std::string& log);

	void SetGameStatus(GameStatus game_status, std::string status);

	ChampInfo GetChampInfoById(int id);

	void GetAllChampInfo();

private:

	std::wstring GetProcessCommandLine(const DWORD& processId);

	ClientInfo GetClientInfo(const DWORD& pid, bool riotClient = false);

	int GetPort(const std::string& cmdLine, bool riotClient = false);
	std::string GetToken(const std::string& cmdLine, bool riotClient = false);
	std::wstring GetProcessPath(const DWORD& processId);
	std::string GetFileVersion(const std::wstring& file);

private:
	ClientInfo client_info_;
	LCURequest lcu_request_;
	OPGGRequest opgg_request_;

	MainWindow* main_window_ = nullptr;

	std::map<int, OPGGChampRankInfo> all_champ_rank_info_;
	std::map<int, ChampInfo> all_champ_info_map_;
	std::map<int, ChampInfo> available_champ_info_map_; //大乱斗我方所有可以使用的英雄

	SummonerInfo cur_summoner_info_;
	ChampInfo cur_select_champ_info_;
	ChampConf cur_champ_conf_;

	GameStatus cur_game_status_ = GameStatus::NoLaunch;
};
