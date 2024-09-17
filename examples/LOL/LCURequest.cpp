#include "stdafx.h"
#include "LCURequest.h"
#include "spdlog/spdlog.h"
#include <map>

void LCURequest::Init(const ClientInfo& client_info)
{
	spdlog::info(__FUNCTION__);

  client_info_ = client_info;
	client_info_.header = MakeLeagueHeader();
}

LoginInfo LCURequest::login()
{
	session_.SetVerifySsl(false);
	session_.SetHeader(StringToHeader(client_info_.header));
	session_.SetUrl(nbase::StringPrintf("https://127.0.0.1:%d/lol-login/v1/session", client_info_.port));
	std::string procSession = session_.Get().text;
	spdlog::info(__FUNCTION__ " result:{}", procSession);

	Json::Value root;
	if (ParseJson(procSession, root))
	{
		login_info_.accountId = root["accountId"].asUInt64();
		login_info_.connected = root["connected"].asBool();
		//login_info_.error = root["error"].asInt();
		login_info_.idToken = root["idToken"].asString();
		login_info_.isInLoginQueue = root["isInLoginQueue"].asBool();
		login_info_.isNewPlayer = root["isNewPlayer"].asBool();
		login_info_.puuid = root["puuid"].asString();
		login_info_.state = root["state"].asString();
		login_info_.summonerId = root["summonerId"].asUInt64();
		login_info_.userAuthToken = root["userAuthToken"].asString();
		login_info_.username = root["username"].asString();
	}

	return login_info_;
}

SummonerInfo LCURequest::GetCurSummonerInfo()
{
	std::string result = Request("get", "/lol-summoner/v1/current-summoner", std::string());
	spdlog::info(__FUNCTION__ " result:{}", result);

	Json::Value root;
	SummonerInfo summoner_info;
	if (ParseJson(result, root) && root.isMember("accountId"))
	{
		summoner_info.accountId = root["accountId"].asUInt64();
		summoner_info.displayName = root["displayName"].asCString();
		summoner_info.gameName = root["gameName"].asCString();
		summoner_info.internalName = root["internalName"].asCString();
		summoner_info.summonerId = root["summonerId"].asUInt64();
		summoner_info.summonerLevel = root["summonerLevel"].asInt();
		summoner_info.tagLine = root["tagLine"].asCString();
	}

	return summoner_info;
}

ChatInfo LCURequest::GetChatInfo()
{
	std::string result = Request("get", "/lol-chat/v1/me", std::string());
	spdlog::info(__FUNCTION__ " result:{}", result);

	Json::Value root;
	ChatInfo chat_info;
	if (ParseJson(result, root))
	{
		chat_info.gameName = root["gameName"].asCString();
		chat_info.gameTag = root["gameTag"].asCString();
		chat_info.championId = root["lol"]["championId"].asCString();
		chat_info.companionId = root["lol"]["companionId"].asCString();
		chat_info.gameQueueType = root["lol"]["gameQueueType"].asCString();
		chat_info.gameStatus = root["lol"]["gameStatus"].asCString();
		chat_info.level = root["lol"]["level"].asCString();
		chat_info.mapId = root["lol"]["mapId"].asCString();
		chat_info.mapSkinId = root["lol"]["mapSkinId"].asCString();
		chat_info.puuid = root["lol"]["puuid"].asCString();
		chat_info.skinname = root["lol"]["skinname"].asCString();
	}

	return chat_info;
}

GameStatus LCURequest::GetGameStatus(std::string& status)
{
	static std::map<std::string, GameStatus> status_map = {
		{"None", GameStatus::None},
		{"Reconnect", GameStatus::Reconnect},
		{"ChampSelect", GameStatus::ChampSelect},
		{"GameStart", GameStatus::GameStart},
		{"InProgress", GameStatus::InProgress},
		{"WaitingForStatus", GameStatus::WaitingForStatus},
		{"EndOfGame", GameStatus::EndOfGame},
		{"Matchmaking", GameStatus::Matchmaking},
		{"Lobby", GameStatus::Lobby},
		{"ReadyCheck", GameStatus::ReadyCheck},
		{"PreEndOfGame", GameStatus::PreEndOfGame},
	};
	std::string result = Request("get", "/lol-gameflow/v1/gameflow-phase", std::string());
	nbase::StringReplaceAll("\"", "", result);

	status = result;
	auto iter = status_map.find(result);
	if (iter != status_map.end())
		return iter->second;

	spdlog::error(__FUNCTION__ " status:{}", result);
	return GameStatus::unknown;
}

MapSideInfo LCURequest::GetMapSideInfo()
{
	std::string result = Request("get", "/lol-champ-select/v1/pin-drop-notification", std::string());
	
	Json::Value root;
	MapSideInfo map_side_info;

	if (ParseJson(result, root))
	{
		map_side_info.mapSide = root["mapSide"].asCString();
		if (root["pinDropSummoners"].isArray())
		{
			for (unsigned int i = 0; i < root["pinDropSummoners"].size(); i++)
			{
				if (root["pinDropSummoners"][i]["isLocalSummoner"].asBool())
				{
					map_side_info.myIndex = i;
					map_side_info.mySlotId = root["pinDropSummoners"][i]["slotId"].asInt();
				}
			}
		}
	}

	return map_side_info;

}

std::vector<ChampSelectInfo> LCURequest::GetChampSelectInfo()
{
	std::string result = Request("get", "/lol-champ-select/v1/session", std::string());

	Json::Value root;
	std::vector<ChampSelectInfo> list;

	if (ParseJson(result, root))
	{
		for (unsigned int i = 0; i < root["myTeam"].size(); i++)
		{
			ChampSelectInfo champ_select_info;
			champ_select_info.assignedPosition = root["myTeam"][i]["assignedPosition"].asCString();
			champ_select_info.championId = root["myTeam"][i]["championId"].asInt();
			champ_select_info.selectedSkinId = root["myTeam"][i]["selectedSkinId"].asInt();
			champ_select_info.summonerId = root["myTeam"][i]["summonerId"].asUInt64();
			list.push_back(champ_select_info);
		}
	}

	return list;
}

std::map<int, ChampInfo> LCURequest::GetAllChampInfoMap()
{
	std::string uri = nbase::StringPrintf("/lol-champions/v1/inventories/%llu/champions-minimal", login_info_.summonerId);
	std::string result = Request("get", uri, std::string());


	spdlog::info(__FUNCTION__ " result:{}", result);

	Json::Value root;
	std::map<int, ChampInfo> champ_info_map;

	if (ParseJson(result, root) && root.isArray())
	{
		for (unsigned int i = 0; i < root.size(); i++)
		{
			ChampInfo champ_info;
			champ_info.id = root[i]["id"].asInt();
			champ_info.name = root[i]["name"].asString();
			champ_info.title = root[i]["title"].asString();
			champ_info.alias = root[i]["alias"].asString();
			champ_info.freeToPlay = root[i]["freeToPlay"].asBool();
			champ_info.squarePortraitPath = root[i]["squarePortraitPath"].asString();

			if (champ_info.id >= 0 && !champ_info.name.empty())
				champ_info_map[champ_info.id] = champ_info;
		}
	}

	return champ_info_map;
}

int LCURequest::GetCurSelectChampionId()
{
	std::string result = Request("get", "/lol-champ-select/v1/current-champion", std::string()); //失败返回0
	
	if (result.empty())
		return 0;

	int id = 0;
	if (nbase::StringToInt(result, &id))
		return id;

	return 0;
}

void LCURequest::SetChampionItem(const ChampConf& champ_conf)
{
	std::string uri = nbase::StringPrintf("/lol-item-sets/v1/item-sets/%llu/sets", login_info_.summonerId);
	std::string get_result = Request("get", uri, std::string());
	Json::Value root;

	if (!ParseJson(get_result, root))
	{
		return;
	}

	Json::Value& item = root["itemSets"][0];
	item["associatedChampions"][0] = champ_conf.champ_id;
	item["associatedMaps"][0] = 12;
	item["map"] = "any";
	item["mode"] = "any";
	item["sortrank"] = 0;
	item["startedFrom"] = "blank";
	item["title"] = champ_conf.version + "-" + champ_conf.champ_name;
	item["type"] = "custom";

	Json::Value& blocks = root["itemSets"][0]["blocks"];
	//核心装备推荐
	unsigned int block_count = 0;
	for (unsigned int i = 0; i < champ_conf.items.size(); i++)
	{
		for (unsigned int j = 0; j < champ_conf.items[i].ids.size(); j++)
		{
			blocks[i]["items"][j]["count"] = 1;
			blocks[i]["items"][j]["id"] = nbase::IntToString(champ_conf.items[i].ids[j]);
		}
		blocks[i]["type"] = nbase::StringPrintf("局数:%d 胜率:%.2f", champ_conf.items[i].play, champ_conf.items[i].win_rate * 100);
		blocks[i]["items"].resize(champ_conf.items[i].ids.size());
		block_count++;
	}


	blocks[block_count]["type"] = "鞋子推荐";
	blocks[block_count]["items"].resize(champ_conf.boot_items.size());
	for (unsigned int i = 0; i < champ_conf.boot_items.size(); i++)
	{
		blocks[block_count]["items"][i]["count"] = 1;
		blocks[block_count]["items"][i]["id"] = nbase::IntToString(champ_conf.boot_items[i].ids[0]);
	}
	block_count++;

	blocks[block_count]["type"] = "其它装备推荐";
	blocks[block_count]["items"].resize(champ_conf.other_items.size());
	for (unsigned int i = 0; i < champ_conf.other_items.size(); i++)
	{
		blocks[block_count]["items"][i]["count"] = 1;
		blocks[block_count]["items"][i]["id"] = nbase::IntToString(champ_conf.other_items[i].ids[0]);
	}

	Json::FastWriter fastWriter;
	std::string content = fastWriter.write(root);

	uri = nbase::StringPrintf("/lol-item-sets/v1/item-sets/%llu/sets", login_info_.summonerId);
	std::string result = Request("put", uri, content);
	spdlog::info(__FUNCTION__ " result:{}", result);
}

void LCURequest::SetChampionPerk(const ChampConf& champ_conf)
{
	std::string get_result = Request("get", "/lol-perks/v1/currentpage", std::string());
	Json::Value get_root;

	if (!ParseJson(get_result, get_root))
	{
		return;
	}

	std::string uri = nbase::StringPrintf("/lol-perks/v1/pages/%d", get_root["id"].asInt());
	std::string result = Request("delete", uri, std::string());

	Json::Value root;
	root["current"] = true;
	root["name"] = nbase::StringPrintf("%s 天赋 胜率:%.2f", champ_conf.champ_name.c_str(), champ_conf.rune_items[0].win_rate * 100);
	root["primaryStyleId"] = champ_conf.rune_items[0].primary_page_id;
	root["subStyleId"] = champ_conf.rune_items[0].secondary_page_id;
	for (unsigned int i = 0; i < champ_conf.rune_items[0].selectedPerkIds.size(); i++)
	{
		root["selectedPerkIds"][i] = champ_conf.rune_items[0].selectedPerkIds[i];
	}

	Json::FastWriter fastWriter;
	std::string content = fastWriter.write(root);


	result = Request("post", "lol-perks/v1/pages", content);
	spdlog::info(__FUNCTION__ " result:{}", result);
}

void LCURequest::DownloadImage(std::string uri)
{
	if (uri.empty())
		return;

	std::string name = uri.substr(uri.rfind("/"));

	std::wstring path = nbase::win32::GetCurrentModuleDirectory() + L"image" + nbase::UTF8ToUTF16(name);

	bool is_directory = false;
	bool exist = nbase::FilePathIsExist(path, is_directory);
	if (exist)
		return;

	std::string get_result = Request("get", uri, std::string());

	nbase::WriteFile(path, get_result);

	spdlog::info(__FUNCTION__ " uri:{}", uri);
}

std::string LCURequest::RequestTest(const std::string& endpoint)
{
	return Request("get", endpoint, std::string());
}

std::string LCURequest::MakeLeagueHeader()
{
	return "Host: 127.0.0.1:" + std::to_string(client_info_.port) + "\r\n" +
		"Connection: keep-alive" + "\r\n" +
		"Authorization: Basic " + client_info_.token + "\r\n" +
		"Accept: application/json" + "\r\n" +
		"Content-Type: application/json" + "\r\n" +
		"Origin: https://127.0.0.1:" + std::to_string(client_info_.port) + "\r\n" +
		"User-Agent: Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) LeagueOfLegendsClient/" + client_info_.version +
		" (CEF 91) Safari/537.36" + "\r\n" +
		"X-Riot-Source: rcp-fe-lol-social" + "\r\n" +
		R"(sec-ch-ua: "Chromium";v="91")" + "\r\n" +
		"sec-ch-ua-mobile: ?0" + "\r\n" +
		"Sec-Fetch-Site: same-origin" + "\r\n" +
		"Sec-Fetch-Mode: cors" + "\r\n" +
		"Sec-Fetch-Dest: empty" + "\r\n" +
		"Referer: https://127.0.0.1:" + std::to_string(client_info_.port) + "/index.html" + "\r\n" +
		"Accept-Encoding: gzip, deflate, br" + "\r\n" +
		"Accept-Language: en-US,en;q=0.9\r\n\r\n";
}

cpr::Header LCURequest::StringToHeader(const std::string& str)
{
	cpr::Header header;
	for (const auto& line : ui::StringHelper::Split(str, std::string("\r\n")))
	{
		if (const auto index = line.find(": ", 0); index != std::string::npos)
		{
			header.insert({ line.substr(0, index), line.substr(index + 2) });
		}
	}
	return header;
}

bool LCURequest::ParseJson(const std::string& str, Json::Value& root)
{
	Json::CharReaderBuilder builder;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	JSONCPP_STRING err;

	if (reader->parse(str.c_str(), str.c_str() + static_cast<int>(str.length()), &root, &err))
		return true;

	return false;
}

std::string LCURequest::Request(const std::string& method, const std::string& endpoint, const std::string& body)
{
	if (client_info_.port == 0)
		return "Not connected to League";

	std::string sURL = endpoint;
	if (sURL.find("https://127.0.0.1") == std::string::npos)
	{
		if (sURL.find("https://") == std::string::npos && sURL.find("http://") == std::string::npos)
		{
			while (sURL[0] == ' ')
				sURL.erase(sURL.begin());
			if (sURL[0] != '/')
				sURL.insert(0, "/");
			sURL.insert(0, "https://127.0.0.1:" + std::to_string(client_info_.port));
		}
	}
	else if (sURL.find("https://127.0.0.1:") == std::string::npos)
	{
		sURL.insert(strlen("https://127.0.0.1"), ":" + std::to_string(client_info_.port));
	}

	cpr::Response r = {};

	session_.SetUrl(sURL);
	session_.SetBody(body);

	std::string upperMethod = method;
	nbase::UpperString(upperMethod);
	if (upperMethod == "GET")
	{
		r = session_.Get();
	}
	else if (upperMethod == "POST")
	{
		r = session_.Post();
	}
	else if (upperMethod == "OPTIONS")
	{
		r = session_.Options();
	}
	else if (upperMethod == "DELETE")
	{
		r = session_.Delete();
	}
	else if (upperMethod == "PUT")
	{
		r = session_.Put();
	}
	else if (upperMethod == "HEAD")
	{
		r = session_.Head();
	}
	else if (upperMethod == "PATCH")
	{
		r = session_.Patch();
	}

	return r.text;
}