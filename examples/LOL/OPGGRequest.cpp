#include "stdafx.h"
#include "OPGGRequest.h"
#include "spdlog/spdlog.h"
#include <map>
#include <algorithm>

void OPGGRequest::Init(std::string client_version)
{
	client_version_ = client_version;

	spdlog::info(__FUNCTION__ " version:{}", client_version_);
}

std::map<int, OPGGChampRankInfo> OPGGRequest::GetOPGGChampRankInfo()
{
	session_.SetUrl(nbase::StringPrintf("https://lol-api-champion.op.gg/api/%s/champions/%s?tier=all&version=%s", 
		"kr", "aram", client_version_.c_str()));
	std::string procSession = session_.Get().text;

	spdlog::info(__FUNCTION__ " result:{}", procSession);

	Json::CharReaderBuilder builder;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	JSONCPP_STRING err;
	Json::Value root;

	std::map<int, OPGGChampRankInfo> map;
	if (reader->parse(procSession.c_str(), procSession.c_str() + static_cast<int>(procSession.length()), &root, &err))
	{
		opgg_version_ = root["meta"]["version"].asString();
		unsigned int size = root["data"].size();
		
		for (unsigned int i = 0; i < size; i++)
		{
			OPGGChampRankInfo rank_info;
			rank_info.id = root["data"][i]["id"].asInt();
			rank_info.tier = root["data"][i]["average_stats"]["tier"].asInt();
			rank_info.rank = root["data"][i]["average_stats"]["rank"].asInt();
			rank_info.win_rate = root["data"][i]["average_stats"]["win_rate"].asFloat();
			map[rank_info.id] = rank_info;
		}
	}


  return map;
}

ChampConf OPGGRequest::GetChampConf(int champ_id)
{
	session_.SetUrl(nbase::StringPrintf("https://lol-api-champion.op.gg/api/%s/champions/%s/%d/none?tier=all&version=%s", 
		"kr", "aram", champ_id, client_version_.c_str()));

	std::string procSession = session_.Get().text;

	Json::CharReaderBuilder builder;
	const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	JSONCPP_STRING err;
	Json::Value root;

	ChampConf champ_conf;
	champ_conf.champ_id = champ_id;
	if (reader->parse(procSession.c_str(), procSession.c_str() + static_cast<int>(procSession.length()), &root, &err))
	{
		for (unsigned int i = 0; i < root["data"]["core_items"].size(); i++)
		{
			CoreItem item;
			for (unsigned int j = 0; j < root["data"]["core_items"][i]["ids"].size(); j++)
			{
				item.ids.push_back(root["data"]["core_items"][i]["ids"][j].asInt());
			}
			item.play = root["data"]["core_items"][i]["play"].asInt();
			item.win = root["data"]["core_items"][i]["win"].asInt();
			item.win_rate = static_cast<double>(item.win) / item.play;
			champ_conf.items.push_back(item);
		}

		for (unsigned int i = 0; i < root["data"]["last_items"].size(); i++)
		{
			CoreItem item;
			for (unsigned int j = 0; j < root["data"]["last_items"][i]["ids"].size(); j++)
			{
				item.ids.push_back(root["data"]["last_items"][i]["ids"][j].asInt());
			}
			item.play = root["data"]["last_items"][i]["play"].asInt();
			item.win = root["data"]["last_items"][i]["win"].asInt();
			item.win_rate = static_cast<double>(item.win) / item.play;
			champ_conf.other_items.push_back(item);
		}

		//天赋
		for (unsigned int i = 0; i < root["data"]["rune_pages"].size(); i++)
		{
			for (unsigned int j = 0; j < root["data"]["rune_pages"][i]["builds"].size(); j++)
			{
				RuneItem item;
				item.primary_page_id = root["data"]["rune_pages"][i]["builds"][j]["primary_page_id"].asInt();
				item.secondary_page_id = root["data"]["rune_pages"][i]["builds"][j]["secondary_page_id"].asInt();

				for (unsigned int k = 0; k < root["data"]["rune_pages"][i]["builds"][j]["primary_rune_ids"].size(); k++)
				{
					item.selectedPerkIds.push_back(root["data"]["rune_pages"][i]["builds"][j]["primary_rune_ids"][k].asInt());
				}

				for (unsigned int k = 0; k < root["data"]["rune_pages"][i]["builds"][j]["secondary_rune_ids"].size(); k++)
				{
					item.selectedPerkIds.push_back(root["data"]["rune_pages"][i]["builds"][j]["secondary_rune_ids"][k].asInt());
				}

				for (unsigned int k = 0; k < root["data"]["rune_pages"][i]["builds"][j]["stat_mod_ids"].size(); k++)
				{
					item.selectedPerkIds.push_back(root["data"]["rune_pages"][i]["builds"][j]["stat_mod_ids"][k].asInt());
				}

				item.play = root["data"]["rune_pages"][i]["builds"][j]["play"].asInt();
				item.win = root["data"]["rune_pages"][i]["builds"][j]["win"].asInt();
				item.win_rate = static_cast<double>(item.win) / item.play;
				champ_conf.rune_items.push_back(item);

			}
		}

		//鞋子
		for (unsigned int i = 0; i < root["data"]["boots"].size(); i++)
		{
			CoreItem item;
			for (unsigned int j = 0; j < root["data"]["boots"][i]["ids"].size(); j++)
			{
				item.ids.push_back(root["data"]["boots"][i]["ids"][j].asInt());
			}
			item.play = root["data"]["boots"][i]["play"].asInt();
			item.win = root["data"]["boots"][i]["win"].asInt();
			item.win_rate = static_cast<double>(item.win) / item.play;
			champ_conf.boot_items.push_back(item);
		}

		champ_conf.version = root["meta"]["version"].asString();
	}




	//把对局数太少的去掉  如果没有场次少不能这么处理
	//std::sort(champ_conf.items.begin(), champ_conf.items.end(), [](const CoreItem& a, const CoreItem& b) {
	//	return a.play > b.play;
	//	});
	//champ_conf.items.resize(champ_conf.items.size() > 10 ? 10 : champ_conf.items.size());

	//std::sort(champ_conf.other_items.begin(), champ_conf.other_items.end(), [](const CoreItem& a, const CoreItem& b) {
	//	return a.play > b.play;
	//	});
	//champ_conf.other_items.resize(champ_conf.other_items.size() > 20 ? 20 : champ_conf.other_items.size());

	//std::sort(champ_conf.rune_items.begin(), champ_conf.rune_items.end(), [](const RuneItem& a, const RuneItem& b) {
	//	return a.play > b.play;
	//	});
	//champ_conf.rune_items.resize(champ_conf.rune_items.size() > 10 ? 10 : champ_conf.rune_items.size());

	//根据胜率排序
	std::sort(champ_conf.items.begin(), champ_conf.items.end(), [](const CoreItem& a, const CoreItem& b) {
		return a.win_rate > b.win_rate;
		});
	champ_conf.items.resize(champ_conf.items.size() > 3 ? 3 : champ_conf.items.size());

	std::sort(champ_conf.other_items.begin(), champ_conf.other_items.end(), [](const CoreItem& a, const CoreItem& b) {
		return a.win_rate > b.win_rate;
		});
	champ_conf.other_items.resize(champ_conf.other_items.size() > 20 ? 20 : champ_conf.other_items.size());

	std::sort(champ_conf.boot_items.begin(), champ_conf.boot_items.end(), [](const CoreItem& a, const CoreItem& b) {
		return a.win_rate > b.win_rate;
		});

	//天赋胜率排序
	std::sort(champ_conf.rune_items.begin(), champ_conf.rune_items.end(), [](const RuneItem& a, const RuneItem& b) {
		return a.win_rate > b.win_rate;
		});


	return champ_conf;
}
