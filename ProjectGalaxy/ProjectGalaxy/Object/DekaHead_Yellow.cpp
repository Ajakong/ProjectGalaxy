#include "DekaHead_Yellow.h"
#include"ModelManager.h"
#include"GraphManager.h"

namespace
{
	const char* kModelName = "CuteCreature_yellow";
	const char* kGraphName = "cuteCreature_yellow.png";
}
DekaHead_Yellow::DekaHead_Yellow(Vec3 pos) : 
	TalkObject(pos,ModelManager::GetInstance().GetModelData(kModelName))
{
}

DekaHead_Yellow::~DekaHead_Yellow()
{
}

std::list<std::list<std::string>> DekaHead_Yellow::GetTexts()
{
	UI::GetInstance().SetTalkObjectHandle(UI::TalkGraphKind::Dekahead_Yellow);
	std::list<std::list<std::string>> talkText;
	std::list<std::string> one;
	one.push_back("隊長じゃん！元気？");
	talkText.push_back(one);

	
	return talkText;
}
