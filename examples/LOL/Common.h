#pragma once

#include <string>

enum ThreadId
{
	kThreadUI,
	kThreadGlobalMisc,
	kThreadGame,
};

struct ClientInfo
{
	int port = 0;
	std::string token;
	std::string header;
	std::string version;
	std::wstring path;
};


struct LoginInfo
{
	uint64_t accountId = 0;
	bool connected = false;
	std::string error;
	std::string idToken;
	bool isInLoginQueue = false;
	bool isNewPlayer = false;
	std::string puuid;
	std::string state;
	uint64_t summonerId = 0;
	std::string userAuthToken;
	std::string username;
};

//召唤师信息
struct SummonerInfo
{
	uint64_t accountId = 0;
	std::string displayName;
	std::string gameName;
	std::string internalName;
	uint64_t summonerId = 0;
	int summonerLevel = 0;
	std::string tagLine;
};


struct ChatInfo
{
	std::string gameName;
	std::string gameTag;
	std::string championId;
	std::string companionId;
	std::string gameQueueType;
	std::string gameStatus;
	std::string level;
	std::string mapId;
	std::string mapSkinId;
	std::string puuid;
	std::string skinname;
};


enum class GameStatus
{
	NoLaunch,
	Launching,
	None,
	Reconnect,
	ChampSelect,
	GameStart,
	InProgress,
	WaitingForStatus,
	EndOfGame,
	Matchmaking,
	Lobby,
	ReadyCheck,  //排到人等待确认
	PreEndOfGame,

	unknown
};

struct MapSideInfo
{
	std::string mapSide;  //blue
	int myIndex = 0;
	int mySlotId = 0;
};

//在选择英雄时的一些信息
struct ChampSelectInfo
{
	std::string assignedPosition;
	int championId = 0;
	int selectedSkinId = 0;
	uint64_t summonerId = 0;
};


struct OPGGChampRankInfo
{
	int id = 0;   //英雄id
	int tier = 0; //强度
	int rank = 0;  //排名
	float win_rate = 0.f;  //胜率
};

//英雄信息
struct ChampInfo
{
	int id = 0;
	std::string name;
	std::string title;
	std::string alias;
	bool freeToPlay = false;
	std::string squarePortraitPath;

	int tier = 0; //强度
	int rank = 0;  //排名
	float win_rate = 0.f;  //胜率

	bool otherSelected = false;
};

//核心装备
struct CoreItem
{
	std::vector<int> ids; //装备id
	int play = 0; //对局数
	int win = 0; //赢的对局数
	float win_rate = 0.f; //胜率
};

//天赋
struct RuneItem
{
	int primary_page_id = 0;
	int secondary_page_id = 0;
	std::vector<int> selectedPerkIds;
	int play = 0; //对局数
	int win = 0; //赢的对局数
	float win_rate = 0.f; //胜率
};

struct ChampConf
{
	int champ_id = 0;
	std::string champ_name;

	std::vector<CoreItem> items;   //核心装备
	std::vector<CoreItem> other_items; //其它推荐装备
	std::vector<CoreItem> boot_items; //鞋子

	std::vector<RuneItem> rune_items;
	std::string version;
};

