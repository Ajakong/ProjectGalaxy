#include "DekaHead_Blue.h"
#include"ModelManager.h"
#include"GraphManager.h"

namespace
{
	const char* kModelName = "CuteCreature_blue";
	const char* kGraphName = "cuteCreature_blue.png";
}
DekaHead_Blue::DekaHead_Blue(Vec3 pos) :
	TalkObject(pos,ModelManager::GetInstance().GetModelData(kModelName),GraphManager::GetInstance().GetGraphData(kGraphName))
{
}

DekaHead_Blue::~DekaHead_Blue()
{
}

std::list<std::list<std::string>> DekaHead_Blue::GetTexts()
{
	std::list<std::string> one;
	one.push_back("Hey!Brother");
	one.push_back("調子はどうだ");
	m_talkTexts.push_back(one);

	std::list<std::string> two;
	two.push_back("俺は最近BLUEなんだ");
	two.push_back("どうやら最近宇宙で悪さしようとしている奴がいるらしい");
	m_talkTexts.push_back(two);

	std::list<std::string> three;
	three.push_back("宇宙が壊れるのは困るぜ、どうにかしたいんだが");
	three.push_back("ソイツは惑星の正面を這う衝撃波で攻撃するそうだ");
	three.push_back("ンな奴どう戦えば・・・");

	m_talkTexts.push_back(three);

	return m_talkTexts;
}
