﻿#include "UI.h"
#include"GraphManager.h"
#include"Game.h"
#include"TextManager.h"
#include"Pad.h"
#include"GameStopManager.h"
#include"TalkObject.h"
namespace
{
	const char* kGraphUIAssetName = "Designer_ui.png";
	const char* kInputAUIName = "PushAbottonForTalk.png";
	const char* kTakasakiTaisaGraphName = "TakasakiTaisa_talk.png";
	
	const UI::UIinfo kIdemBoxUIInfo{ 0,0,255,255 };
	const UI::UIinfo kHPBarUIInfo { 125,730,820,140 };
	const UI::UIinfo kWindowScreenUIInfo{ 620,15,400,500 };
	const UI::UIinfo kInputAUIInfo{ 265,275,470,470 };
	const UI::UIinfo kTalkingCharaGraph{ 0,0,775,890 };

	constexpr float kHpDecreaseSpeed = 0.3f;

	constexpr int lTextBoxDrawInterval = 60;
}

UI::UI():
	m_fadeSpeed(1),
	m_appearFrame(0)
{
	m_takasakiTaisaHandle = GraphManager::GetInstance().GetGraphData(kTakasakiTaisaGraphName);
	m_textManager = std::make_shared<TextManager>();
}

UI::~UI()
{
}

UI& UI::GetInstance()
{
	static UI info;
	return info;
}

void UI::Init()
{
	m_uiAssetHandle = GraphManager::GetInstance().GetGraphData(kGraphUIAssetName);
	m_uiInputAHandle = GraphManager::GetInstance().GetGraphData(kInputAUIName);
	m_uiUpdate = &UI::NormalUpdate;
	m_uiDraw = &UI::NormalDraw;
	m_appearFrame = 0;
}

void UI::Update()
{
	
	(this->*m_uiUpdate)();

}

void UI::NormalUpdate()
{
	if (m_textManager->GetTextDataSize() != 0)
	{
		m_talkingCharaHandle = m_takasakiTaisaHandle;
		m_fadeSpeed = 1;
		Pad::SetState("TextInput");
		m_uiUpdate = &UI::AppaerUpdate;
		m_uiDraw = &UI::TextBoxFadeDraw;
	}
	
}

void UI::AppaerUpdate()
{
	m_appearFrame+=m_fadeSpeed;

	if (m_appearFrame >= lTextBoxDrawInterval&&m_uiDraw==&UI::TextBoxFadeDraw)
	{
		
		m_uiUpdate = &UI::TextBoxUpdate;
		m_uiDraw = &UI::TextBoxDraw;
	}
	if (m_appearFrame >= 60 && m_uiDraw == &UI::InputAFadeDraw)
	{
		
		m_uiUpdate = &UI::InputAUpdate;
		m_uiDraw = &UI::InputADraw;
	}
}

void UI::TextBoxUpdate()
{
	//Aでテキストを進める
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		DeleteText();
	}
	//表示させるテキストがない場合
	if (m_textManager->GetTextDataSize() == 0)
	{
		//テキストボックスのフェードアウトヘ以降
		m_uiUpdate = &UI::FadeOutUpdate;
		m_uiDraw = &UI::TextBoxFadeDraw;
	}
}

void UI::InputAUpdate()
{
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		Pad::SetState("TextInput");
		m_textManager->SetTexts(m_nowTalkObject->GetTexts());
		m_appearFrame = 0;
		m_uiUpdate = &UI::AppaerUpdate;
		m_uiDraw = &UI::TextBoxFadeDraw;
	}
}

void UI::FadeOutUpdate()
{
	m_appearFrame-=m_fadeSpeed;
	if (m_appearFrame <= 0)
	{
		Pad::SetState("PlayerInput");
		m_uiUpdate = &UI::NormalUpdate;
		m_uiDraw = &UI::NormalDraw;
	}
}

void UI::Draw(float m_hp)
{
	m_playerHp = m_hp;

	if (m_playerHp > 0)
	{
		DrawBox(40, 40, 780, kHPBarUIInfo.height / 2 + 10, 0x0000044, true);
		DrawBox(40, 40, 40.f + 15.f * static_cast<int>(m_playerHp), kHPBarUIInfo.height / 2 + 10, 0x00044ff, true);

		DrawRectRotaGraph(static_cast<int>(kHPBarUIInfo.width / 2), static_cast<int>(kHPBarUIInfo.height / 2), kHPBarUIInfo.x, kHPBarUIInfo.y, kHPBarUIInfo.width, kHPBarUIInfo.height, 1, 0, m_uiAssetHandle, true);
#ifdef DEBUG
		DrawRectRotaGraph(Game::kScreenWidth - kIdemBoxUIInfo.width / 2 - 170, kIdemBoxUIInfo.height / 2 + 50, kIdemBoxUIInfo.x, kIdemBoxUIInfo.y, kIdemBoxUIInfo.width, kIdemBoxUIInfo.height, 1, 0, m_uiAssetHandle, true);
#endif

	}
	(this->*m_uiDraw)();
	
}

void UI::NormalDraw()
{
	
#ifdef DEBUG
	DrawRectRotaGraph(Game::kScreenWidth - kIdemBoxUIInfo.width / 2 - 170, kIdemBoxUIInfo.height / 2 + 50, kIdemBoxUIInfo.x, kIdemBoxUIInfo.y, kIdemBoxUIInfo.width, kIdemBoxUIInfo.height, 1, 0, m_uiAssetHandle, true);
#endif
	/*DrawRectRotaGraph( Game::kScreenWidth-kWindowScreenUIInfo.width / 2, kWindowScreenUIInfo.height / 2, kWindowScreenUIInfo.x, kWindowScreenUIInfo.y, kWindowScreenUIInfo.width, kWindowScreenUIInfo.height, 1, 0, m_uiAssetHandle, true);*/

}

void UI::InputAFadeDraw()
{
	DrawRectRotaGraph(Game::kScreenWidth/2,Game::kScreenHeight/2, kInputAUIInfo.x, kInputAUIInfo.y, kInputAUIInfo.width, kInputAUIInfo.height, m_appearFrame*0.002f, 0, m_uiInputAHandle, true);
}

void UI::InputADraw()
{
	DrawRectRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, kInputAUIInfo.x, kInputAUIInfo.y, kInputAUIInfo.width, kInputAUIInfo.height, m_appearFrame * 0.002f, 0, m_uiInputAHandle, true);
}

void UI::TextBoxFadeDraw()
{
	DrawBox(0, 0, Game::kScreenWidth, m_appearFrame, 0x000000, true);
	DrawBox(0, Game::kScreenHeight, Game::kScreenWidth, Game::kScreenHeight-m_appearFrame, 0x000000, true);


	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	DrawBox(Game::kScreenWidth / 2 - m_appearFrame * 9, 90, Game::kScreenWidth / 2 + m_appearFrame * 9, 440, 0x111111, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(Game::kScreenWidth / 2 - m_appearFrame * 9, 90, Game::kScreenWidth / 2 + m_appearFrame * 9, 440, 0x0000ff, false);
}

void UI::TextBoxDraw()
{
	DrawBox(0, 0, Game::kScreenWidth, m_appearFrame, 0x000000, true);
	DrawBox(0, Game::kScreenHeight, Game::kScreenWidth, Game::kScreenHeight - m_appearFrame, 0x000000, true);
	
	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	DrawBox(Game::kScreenWidth / 2 - m_appearFrame * 9, 90, Game::kScreenWidth / 2 + m_appearFrame * 9, 440, 0x111111, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(Game::kScreenWidth / 2 - m_appearFrame * 9, 90, Game::kScreenWidth / 2 + m_appearFrame * 9, 440, 0x0000ff, false);

	DrawRectRotaGraph(static_cast<int>(kWindowScreenUIInfo.width / 2) + 950, static_cast<int>(kWindowScreenUIInfo.height / 2)+20, kTalkingCharaGraph.x, kTalkingCharaGraph.y, kTalkingCharaGraph.width, kTalkingCharaGraph.height, 0.3f, 0, m_talkingCharaHandle, true);

	DrawRectRotaGraph(static_cast<int>(kWindowScreenUIInfo.width / 2)+950, static_cast<int>(kWindowScreenUIInfo.height / 2)+50, kWindowScreenUIInfo.x, kWindowScreenUIInfo.y, kWindowScreenUIInfo.width, kWindowScreenUIInfo.height, 0.7f, 0, m_uiAssetHandle, true);
	m_textManager->Draw();
}

void UI::InText(const std::string text)
{
	m_textManager->InText(text);
}

void UI::InTexts(const std::list<std::string> texts)
{
	m_textManager->InTexts(texts);
}

void UI::WannaTalk(std::shared_ptr<TalkObject> obj,int graphHandle)
{
	m_fadeSpeed = 20;
	m_nowTalkObject = obj;
	m_talkingCharaHandle = graphHandle;
	m_uiUpdate = &UI::AppaerUpdate;
	m_uiDraw = &UI::InputAFadeDraw;
}

void UI::TalkExit()
{
	//テキストボックスのフェードアウトヘ以降
	m_uiUpdate = &UI::FadeOutUpdate;
	m_uiDraw = &UI::InputAFadeDraw;
}

void UI::DeleteText()
{
	m_textManager->DeleteText();
}

int UI::TextRemaining()
{
	return m_textManager->GetTextDataSize();
}
