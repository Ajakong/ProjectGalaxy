#include "DekaHead_Red.h"
#include"ModelManager.h"
#include"GraphManager.h"

namespace
{
	const char* kModelName = "CuteCreature_red";
	const char* kGraphName = "cuteCreature_red.png";
}

DekaHead_Red::DekaHead_Red(Vec3 pos) : 
	TalkObject(pos,ModelManager::GetInstance().GetModelData(kModelName),GraphManager::GetInstance().GetGraphData(kGraphName))
{
}

DekaHead_Red::~DekaHead_Red()
{
}

std::list<std::list<std::string>> DekaHead_Red::GetTexts()
{
	std::list<std::string> one;
	one.push_back("俺は感度バンバーンだぜ");
	one.push_back("おまえが隊長だな");
	m_talkTexts.push_back(one);

	std::list<std::string> two;
	two.push_back("タカサキ大佐から話は聞いてるぜ");
	m_talkTexts.push_back(two);

	std::list<std::string> three;
	three.push_back("久しぶりの任務で体がなまってるだろうから");
	three.push_back("まずは金色のタマをとって体をうごかせってよ");
	m_talkTexts.push_back(three);

	return m_talkTexts;
}
