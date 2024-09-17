#pragma once

#include "Common.h"
#include <cpr/cpr.h>
#include <json/json.h>
#include <vector>

class OPGGRequest
{
public:
  void Init(std::string client_version);

  std::map<int, OPGGChampRankInfo> GetOPGGChampRankInfo();

  ChampConf GetChampConf(int champ_id);

private:
  cpr::Session session_;
  std::string opgg_version_;
  std::string client_version_;
};
