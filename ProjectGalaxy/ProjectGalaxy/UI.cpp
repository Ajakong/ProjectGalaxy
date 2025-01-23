#include "UI.h"
#include"GraphManager.h"
#include"Game.h"
#include"TextManager.h"
#include"Pad.h"
namespace
{
	const char* kGraphUIAssetName = "Designer_ui.png";
	const UI::UIinfo kIdemBoxUIInfo{ 0,0,255,255 };
	const UI::UIinfo kHPBarUIInfo { 125,730,820,140 };
	const UI::UIinfo kWindowScreenUIInfo{ 620,15,400,500 };

	constexpr float kHpDecreaseSpeed = 0.3f;

	constexpr int lTextBoxDrawInterval = 60;
}

UI::UI():
	m_textBoxFrame(0)
{
	m_uiAssetHandle = GraphManager::GetInstance().GetGraphData(kGraphUIAssetName);
	m_uiUpdate = &UI::NormalUpdate;
	m_uiDraw = &UI::NormalDraw;
	m_textManager = std::make_shared<TextManager>();
}

UI::~UI()
{
}

void UI::Init()
{
}

void UI::Update()
{
	
	(this->*m_uiUpdate)();

}

void UI::NormalUpdate()
{
	if (m_textManager->GetTextDataSize() != 0)
	{
		Pad::SetState("TextInput");
		m_uiUpdate = &UI::AppaerUpdate;
		m_uiDraw = &UI::FadeDraw;
	}
}

void UI::AppaerUpdate()
{
	m_textBoxFrame++;
	if (m_textBoxFrame >= lTextBoxDrawInterval)
	{
		m_uiUpdate = &UI::TextBoxUpdate;
		m_uiDraw = &UI::TextBoxDraw;
	}
}

void UI::TextBoxUpdate()
{
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		DeleteText();
	}
	if (m_textManager->GetTextDataSize() == 0)
	{

		m_uiUpdate = &UI::FadeOutUpdate;
		m_uiDraw = &UI::FadeDraw;
	}
}

void UI::FadeOutUpdate()
{
	m_textBoxFrame--;
	if (m_textBoxFrame <= 0)
	{
		Pad::SetState("PlayerInput");
		m_uiUpdate = &UI::NormalUpdate;
		m_uiDraw = &UI::NormalDraw;
	}
}

void UI::Draw(float m_hp)
{
	m_playerHp = m_hp;
	(this->*m_uiDraw)();
	
}

void UI::NormalDraw()
{
	DrawBox(40, 40, 780, kHPBarUIInfo.height / 2 + 10, 0x0000044, true);
	DrawBox(40, 40, 40.f + 15.f * static_cast<int>(m_playerHp), kHPBarUIInfo.height / 2 + 10, 0x00044ff, true);

	DrawRectRotaGraph(static_cast<int>(kHPBarUIInfo.width / 2), static_cast<int>(kHPBarUIInfo.height / 2), kHPBarUIInfo.x, kHPBarUIInfo.y, kHPBarUIInfo.width, kHPBarUIInfo.height, 1, 0, m_uiAssetHandle, true);
#ifdef DEBUG
	DrawRectRotaGraph(Game::kScreenWidth - kIdemBoxUIInfo.width / 2 - 170, kIdemBoxUIInfo.height / 2 + 50, kIdemBoxUIInfo.x, kIdemBoxUIInfo.y, kIdemBoxUIInfo.width, kIdemBoxUIInfo.height, 1, 0, m_uiAssetHandle, true);
#endif
	/*DrawRectRotaGraph( Game::kScreenWidth-kWindowScreenUIInfo.width / 2, kWindowScreenUIInfo.height / 2, kWindowScreenUIInfo.x, kWindowScreenUIInfo.y, kWindowScreenUIInfo.width, kWindowScreenUIInfo.height, 1, 0, m_uiAssetHandle, true);*/

}

void UI::FadeDraw()
{
	DrawBox(40, 40, 780, kHPBarUIInfo.height / 2 + 10, 0x0000044, true);
	DrawBox(40, 40, 40.f + 15.f * static_cast<int>(m_playerHp), kHPBarUIInfo.height / 2 + 10, 0x00044ff, true);

	DrawRectRotaGraph(static_cast<int>(kHPBarUIInfo.width / 2), static_cast<int>(kHPBarUIInfo.height / 2), kHPBarUIInfo.x, kHPBarUIInfo.y, kHPBarUIInfo.width, kHPBarUIInfo.height, 1, 0, m_uiAssetHandle, true);
#ifdef DEBUG
	DrawRectRotaGraph(Game::kScreenWidth - kIdemBoxUIInfo.width / 2 - 170, kIdemBoxUIInfo.height / 2 + 50, kIdemBoxUIInfo.x, kIdemBoxUIInfo.y, kIdemBoxUIInfo.width, kIdemBoxUIInfo.height, 1, 0, m_uiAssetHandle, true);
#endif
	/*DrawRectRotaGraph( Game::kScreenWidth-kWindowScreenUIInfo.width / 2, kWindowScreenUIInfo.height / 2, kWindowScreenUIInfo.x, kWindowScreenUIInfo.y, kWindowScreenUIInfo.width, kWindowScreenUIInfo.height, 1, 0, m_uiAssetHandle, true);*/
	
	DrawBox(Game::kScreenWidth / 2 - m_textBoxFrame*9, 90, Game::kScreenWidth / 2 + m_textBoxFrame*9, 440,0x004444,true);
	DrawBox(Game::kScreenWidth / 2 - m_textBoxFrame * 9, 90, Game::kScreenWidth / 2 + m_textBoxFrame * 9, 440, 0x444444, false);

	
}

void UI::TextBoxDraw()
{
	DrawBox(40, 40, 780, kHPBarUIInfo.height / 2 + 10, 0x0000044, true);
	DrawBox(40, 40, 40.f + 15.f * static_cast<int>(m_playerHp), kHPBarUIInfo.height / 2 + 10, 0x00044ff, true);

	DrawRectRotaGraph(static_cast<int>(kHPBarUIInfo.width / 2), static_cast<int>(kHPBarUIInfo.height / 2), kHPBarUIInfo.x, kHPBarUIInfo.y, kHPBarUIInfo.width, kHPBarUIInfo.height, 1, 0, m_uiAssetHandle, true);
#ifdef DEBUG
	DrawRectRotaGraph(Game::kScreenWidth - kIdemBoxUIInfo.width / 2 - 170, kIdemBoxUIInfo.height / 2 + 50, kIdemBoxUIInfo.x, kIdemBoxUIInfo.y, kIdemBoxUIInfo.width, kIdemBoxUIInfo.height, 1, 0, m_uiAssetHandle, true);
#endif
	/*DrawRectRotaGraph( Game::kScreenWidth-kWindowScreenUIInfo.width / 2, kWindowScreenUIInfo.height / 2, kWindowScreenUIInfo.x, kWindowScreenUIInfo.y, kWindowScreenUIInfo.width, kWindowScreenUIInfo.height, 1, 0, m_uiAssetHandle, true);*/
	DrawBox(Game::kScreenWidth / 2 - m_textBoxFrame * 9, 90, Game::kScreenWidth / 2 + m_textBoxFrame * 9, 440, 0x004444, true);
	DrawBox(Game::kScreenWidth / 2 - m_textBoxFrame * 9, 90, Game::kScreenWidth / 2 + m_textBoxFrame * 9, 440, 0x444444, false);

	m_textManager->Draw();
}

void UI::InText(const std::wstring text)
{
	m_textManager->InText(text);
}

void UI::DeleteText()
{
	m_textManager->DeleteText();
}
