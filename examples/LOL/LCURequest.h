#pragma once

#include "Common.h"
#include <cpr/cpr.h>
#include <json/json.h>

class LCURequest
{
public:
	void Init(const ClientInfo& client_info);

	LoginInfo login();

	SummonerInfo GetCurSummonerInfo();

	ChatInfo GetChatInfo();

	GameStatus GetGameStatus(std::string& status);

	MapSideInfo GetMapSideInfo();

	std::vector<ChampSelectInfo> GetChampSelectInfo();

	//获取所有英雄信息
	std::map<int, ChampInfo> GetAllChampInfoMap();

	int GetCurSelectChampionId();   //大于0是有效值

	void SetChampionItem(const ChampConf& champ_conf);  //设置推荐装备

	void SetChampionPerk(const ChampConf& champ_conf);  //设置推荐装备

	void DownloadImage(std::string uri);

	std::string RequestTest(const std::string& endpoint);
private:

	std::string MakeLeagueHeader();

	cpr::Header StringToHeader(const std::string& str);

	bool ParseJson(const std::string& str, Json::Value& root);

	std::string Request(const std::string& method, const std::string& endpoint, const std::string& body);

private:
	ClientInfo client_info_;

	cpr::Session session_;

	LoginInfo login_info_;
};
