#pragma once
#include"Collidable.h"
#include<list>
#include<string>

class TalkObject : public MyEngine::Collidable
{
public:
	TalkObject(Vec3 pos);
	virtual ~TalkObject();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	std::list<std::list<std::string>> GetTexts() { return m_talkTexts; }
	
	using talkObjectState_t = void(TalkObject::*)();

	talkObjectState_t m_update;
	talkObjectState_t m_draw;
	virtual void CanWeJustTalk();
	virtual void WaitingUpdate();

	virtual void NormalDraw();

	virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag);
	virtual void OnTriggerStay(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag);
	virtual void OnTriggerExit(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag);

private:
	std::list<std::list<std::string>> m_talkTexts;

	bool m_canTalk;
};

