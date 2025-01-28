#include "DekaHead_Green.h"
#include"ModelManager.h"
#include"GraphManager.h"

namespace
{
	const char* kModelName = "CuteCreature_green";
	const char* kGraphName = "cuteCreature_green.png";
}
DekaHead_Green::DekaHead_Green(Vec3 pos) :
    TalkObject(pos,ModelManager::GetInstance().GetModelData(kModelName),GraphManager::GetInstance().GetGraphData(kGraphName))
{
}

DekaHead_Green::~DekaHead_Green()
{
}

std::list<std::list<std::string>> DekaHead_Green::GetTexts()
{
	std::list<std::string> one;
	one.push_back("ムスコはカワイイかぁ？");
	m_talkTexts.push_back(one);


	return m_talkTexts;
}
