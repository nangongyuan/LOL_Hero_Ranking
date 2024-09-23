#pragma once

#include "LOLGame.h"


class MainWindow : public ui::WindowImplBase
{
public:
	MainWindow();
	~MainWindow();

	void SetSummonerInfo(const SummonerInfo summoner_info);
	void SetChampInfo(const ChampInfo champ_info);
	void SetGameLog(const std::string log);
	void SetAvailableChamp(const std::map<int, ChampInfo> available_champ);

	/**
	 * 一下三个接口是必须要覆写的接口，父类会调用这三个接口来构建窗口
	 * GetSkinFolder		接口设置你要绘制的窗口皮肤资源路径
	 * GetSkinFile			接口设置你要绘制的窗口的 xml 描述文件
	 * GetWindowClassName	接口设置窗口唯一的类名称
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;
	virtual std::wstring GetXmlLayout() override;

	/**
	 * 收到 WM_CREATE 消息时该函数会被调用，通常做一些控件初始化的操作
	 */
	virtual void InitWindow() override;

	/**
	 * 收到 WM_CLOSE 消息时该函数会被调用
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * 标识窗口 class name
	 */
	static const std::wstring kClassName;

private:
	bool OnClickRefresh(ui::EventArgs* arg);
	bool OnClickTest(ui::EventArgs* arg);
	bool OnClickConfigChamp(ui::EventArgs* arg);
private:
	ui::Label* label_summoner_info_ = nullptr;
	ui::Control* control_champ_image_ = nullptr;
	ui::Label* label_champ_name_ = nullptr;
	ui::Label* label_champ_title_ = nullptr;
	ui::Label* label_champ_tier_ = nullptr;
	ui::Label* label_champ_rank_ = nullptr;
	ui::Label* label_win_rate_ = nullptr;

	ui::Button* btn_refresh_ = nullptr;
	ui::Button* btn_test_ = nullptr;
	ui::Button* btn_champ_config_ = nullptr;

	ui::ListBox* champ_list_ = nullptr;

	ui::RichEdit* edit_body_ = nullptr;

	std::unique_ptr<LOLGame> game_;
};

