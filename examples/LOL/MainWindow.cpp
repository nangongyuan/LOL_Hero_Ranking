#include "stdafx.h"
#include "MainWindow.h"
#include "spdlog/spdlog.h"
#include <fstream>

const std::wstring MainWindow::kClassName = L"MainWindow";

MainWindow::MainWindow()
{
  game_ = std::make_unique<LOLGame>(this);
}


MainWindow::~MainWindow()
{
}

void MainWindow::SetSummonerInfo(const SummonerInfo summoner_info)
{
  label_summoner_info_->SetUTF8Text(nbase::StringPrintf("%s (%d)", summoner_info.displayName.c_str(), summoner_info.summonerLevel));

  spdlog::info(__FUNCTION__ " summoner_info: accountId:{} displayName:{}", summoner_info.accountId, summoner_info.displayName);
}

void MainWindow::SetChampInfo(const ChampInfo champ_info)
{
  if (champ_info.name.empty()) return;

  control_champ_image_->SetBkImage(nbase::win32::GetCurrentModuleDirectory() + L"image\\" + nbase::StringPrintf(L"%d.png", champ_info.id));
  label_champ_name_->SetUTF8Text(champ_info.name);
  label_champ_title_->SetUTF8Text(champ_info.title);

  label_champ_tier_->SetUTF8Text(nbase::StringPrintf("强度：T%d", champ_info.tier));
  label_champ_rank_->SetUTF8Text(nbase::StringPrintf("排名：%d", champ_info.rank));
  label_win_rate_->SetUTF8Text(nbase::StringPrintf("胜率：%.2f", champ_info.win_rate * 100));

  spdlog::info(__FUNCTION__ " champ_info: id:{} name:{}", champ_info.id, champ_info.name);
}

void MainWindow::SetGameLog(const std::string log)
{
  edit_body_->SetUTF8Text(log);
}

void MainWindow::SetAvailableChamp(const std::map<int, ChampInfo> available_champ)
{
  std::wstring xml = LR"(
<?xml version="1.0" encoding="UTF-8"?>
<Window>
  <ListContainerElement class="listitem" height="auto">
      <HBox height="auto" padding="10,5,10,5">
        <Button width="20" height="20" name="btn_available" margin="0,0,10,0"/>
        <Control width="30" height="30" name="image"/>
        <VBox height="auto" margin="10,0,0,0">
          <Label name="label_champ_name" text="名字"/>
          <Label name="label_champ_title" text="称呼"/>
        </VBox>
        <VBox>
          <Label name="label_champ_tier" text="强度"/>
          <Label name="label_champ_rank" text="排名"/>
        </VBox>
        <Label text="胜率: " name="label_win_rate"/>
      </HBox>
  </ListContainerElement>
</Window>
        )";
  xml = nbase::StringTrim(xml);

  champ_list_->RemoveAll();

  std::vector<ChampInfo> values;
  for (const auto& pair : available_champ)
    values.push_back(pair.second);

  std::sort(values.begin(), values.end(), [](const ChampInfo& a, const ChampInfo& b) {
    return a.rank < b.rank;
    });

  spdlog::info(__FUNCTION__ " values size:{}", values.size());

  for (const auto& item : values)
  {
    
    ui::ListContainerElement* element = new ui::ListContainerElement;
    ui::GlobalManager::FillBox(element, xml);

    auto btn_available = dynamic_cast<ui::Button*>(element->FindSubControl(L"btn_available"));
    btn_available->SetBkColor(item.otherSelected ? L"red" : L"blue");
    auto image = dynamic_cast<ui::Control*>(element->FindSubControl(L"image"));
    std::wstring image_path = nbase::win32::GetCurrentModuleDirectory() + L"image\\" + nbase::StringPrintf(L"%d.png", item.id);
    bool is_directory = false;
    if (nbase::FilePathIsExist(image_path, is_directory))
      image->SetBkImage(image_path);
    auto label_champ_name = dynamic_cast<ui::Label*>(element->FindSubControl(L"label_champ_name"));
    label_champ_name->SetUTF8Text(item.name);
    auto label_champ_title = dynamic_cast<ui::Label*>(element->FindSubControl(L"label_champ_title"));
    label_champ_title->SetUTF8Text(item.title);
    auto label_champ_tier = dynamic_cast<ui::Label*>(element->FindSubControl(L"label_champ_tier"));
    label_champ_tier->SetUTF8Text(nbase::StringPrintf("强度：T%d", item.tier));
    auto label_champ_rank = dynamic_cast<ui::Label*>(element->FindSubControl(L"label_champ_rank"));
    label_champ_rank->SetUTF8Text(nbase::StringPrintf("排名：%d", item.rank));
    auto label_win_rate = dynamic_cast<ui::Label*>(element->FindSubControl(L"label_win_rate"));
    label_win_rate->SetUTF8Text(nbase::StringPrintf("胜率：%.2f", item.win_rate * 100));
    champ_list_->Add(element);

    spdlog::info(__FUNCTION__ " item name:{} title:{} tier:{} rank:{}", item.name, item.title, item.tier, item.rank);
  }
}

std::wstring MainWindow::GetSkinFolder()
{
	return L"";
}

std::wstring MainWindow::GetSkinFile()
{
	return L"";
}

std::wstring MainWindow::GetWindowClassName() const
{
	return kClassName;
}

ui::Control* MainWindow::CreateControl(const std::wstring& pstrClass)
{

  return nullptr;
}

std::wstring MainWindow::GetXmlLayout()
{
	std::wstring xml = LR"(
<?xml version="1.0" encoding="UTF-8"?>
<Window size="800,700" caption="0,0,0,35">
  <VBox bkcolor="bk_wnd_darkcolor">
    <HBox width="stretch" height="35" bkcolor="bk_wnd_lightcolor">
      <Control width="auto" height="auto" valign="center" margin="8"/>
      <Label text="Controls" valign="center" margin="8"/>
      <Control />
      <Button class="btn_wnd_settings" name="settings" margin="4,6,0,0"/>
      <Button class="btn_wnd_min" name="minbtn" margin="4,6,0,0"/>
      <Box width="21" margin="4,6,0,0">
        <Button class="btn_wnd_max" name="maxbtn"/>
        <Button class="btn_wnd_restore" name="restorebtn" visible="false"/>
      </Box>
      <Button class="btn_wnd_close" name="closebtn" margin="4,6,8,0"/>
    </HBox>
    <VBox padding="30,30,30,30">
      <HBox height="40">
        <Label text="召唤师信息: "/>
        <Label name="label_summoner_info" margin="10,0,0,0"/>
      </HBox>
      <HBox height="60">
        <Control width="50" height="50" name="champ_image"/>
        <VBox margin="10,0,0,0">
          <Label name="label_champ_name" text="名字"/>
          <Label name="label_champ_title" text="称呼"/>
        </VBox>
        <VBox>
          <Label name="label_champ_tier" text="强度"/>
          <Label name="label_champ_rank" text="排名"/>
        </VBox>
        <VBox>
          <Label text="胜率: " name="label_win_rate"/>
        </VBox>
        <VBox>
          <Button class="btn_global_white_80x30" text="应用" name="btn_apply"/>
        </VBox>
      </HBox>
      <VListBox class="list" name="champ_list" padding="5,3,5,3">
      </VListBox>
      <HBox height="40" padding="5,3,5,3">
        <Button class="btn_global_white_80x30" text="初始化" name="btn_refresh"/>
        <Button class="btn_global_white_80x30" text="测试" name="btn_test" margin="5,0,0,0"/>
      </HBox>
      <RichEdit name="edit_body" bkcolor="bk_wnd_lightcolor" width="stretch" height="stretch"
                  multiline="true" vscrollbar="true" hscrollbar="true" autovscroll="true"
                  normaltextcolor="darkcolor" wantreturnmsg="true" rich="true" height="40"/>
    </VBox>
  </VBox>
</Window>

        )";
	return xml;
}

void MainWindow::InitWindow()
{
  label_summoner_info_ = dynamic_cast<ui::Label*>(FindControl(L"label_summoner_info"));
  control_champ_image_ = dynamic_cast<ui::Control*>(FindControl(L"champ_image"));
  label_champ_name_ = dynamic_cast<ui::Label*>(FindControl(L"label_champ_name"));
  label_champ_title_ = dynamic_cast<ui::Label*>(FindControl(L"label_champ_title"));
  label_champ_tier_ = dynamic_cast<ui::Label*>(FindControl(L"label_champ_tier"));
  label_champ_rank_ = dynamic_cast<ui::Label*>(FindControl(L"label_champ_rank"));
  label_win_rate_ = dynamic_cast<ui::Label*>(FindControl(L"label_win_rate"));

  btn_refresh_ = dynamic_cast<ui::Button*>(FindControl(L"btn_refresh"));
  btn_refresh_->AttachClick(std::bind(&MainWindow::OnClickRefresh, this, std::placeholders::_1));

  btn_test_ = dynamic_cast<ui::Button*>(FindControl(L"btn_test"));
  btn_test_->AttachClick(std::bind(&MainWindow::OnClickTest, this, std::placeholders::_1));

  btn_champ_config_ = dynamic_cast<ui::Button*>(FindControl(L"btn_apply"));
  btn_champ_config_->AttachClick(std::bind(&MainWindow::OnClickConfigChamp, this, std::placeholders::_1));

  champ_list_ = dynamic_cast<ui::ListBox*>(FindControl(L"champ_list"));

  edit_body_ = dynamic_cast<ui::RichEdit*>(FindControl(L"edit_body"));

  game_->Start();
}

LRESULT MainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  game_->Stop();
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

bool MainWindow::OnClickRefresh(ui::EventArgs* arg)
{
  return false;
}

bool MainWindow::OnClickTest(ui::EventArgs* arg)
{
  //edit_body_->SetUTF8Text(game_->GetLCURequest().RequestTest(edit_uri_->GetUTF8Text()));
  
  //game_->GetCurrentSelectchampion();

  game_->Test();
  return false;
}

bool MainWindow::OnClickConfigChamp(ui::EventArgs* arg)
{
  nbase::ThreadManager::PostTask(kThreadGame,
    nbase::Bind(&LOLGame::SetChampionConf, game_.get()));
  return false;
}


