#include "UI.h"
#include"GraphManager.h"
namespace
{
	const char* kGraphUIAsset3Name = "UI_asset3.png";
	const UI::UIinfo kSearchUIInfo { 715,400,240,240 };
}
UI::UI()
{
	m_uiAsset3Handle = GraphManager::GetInstance().GetGraphData(kGraphUIAsset3Name);

}

UI::~UI()
{
}

void UI::Init()
{
}

void UI::Update()
{
}

void UI::Draw()
{
	//DrawRectRotaGraph(kSearchUIInfo.width/2, kSearchUIInfo.height/2, kSearchUIInfo.x, kSearchUIInfo.y, kSearchUIInfo.width, kSearchUIInfo.height, 1, 0, m_uiAsset3Handle, false);
}
