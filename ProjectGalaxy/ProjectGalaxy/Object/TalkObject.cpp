#include "TalkObject.h"
#include"ColliderSphere.h"
#include"UI.h"

namespace
{
	constexpr float kTalkingRadius = 20.f;
	constexpr float kRadius = 5.f;

}

TalkObject::TalkObject(Vec3 pos) : Collidable(Priority::High, ObjectTag::TalkObject),
m_canTalk(false)
{

	m_rigid->SetPos(pos);
	{
		AddCollider(MyEngine::ColliderBase::Kind::Sphere, ColideTag::Body);
		auto col = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back()->col);
		col->radius = kRadius;
	}

	{
		AddCollider(MyEngine::ColliderBase::Kind::Sphere, ColideTag::one);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back()->col);
		item->radius = kTalkingRadius;
		item->m_isTrigger = true;
		
	}

	m_update = &TalkObject::WaitingUpdate;
	m_draw = &TalkObject::NormalDraw;

	std::list<std::string> one;
	one.push_back("俺は感度バンバーンだぜ");
	one.push_back("おまえだれだよ");
	m_talkTexts.push_back(one);

	std::list<std::string> two;
	two.push_back("お前がそうなのか！タカサキ大佐から話は聞いてるぜ");
	m_talkTexts.push_back(two);

	std::list<std::string> three;
	three.push_back("久しぶりの任務で体がなまってるだろうから");
	three.push_back("トレーニングをしてくれとのことだ");
	m_talkTexts.push_back(three);
}

TalkObject::~TalkObject()
{
}

void TalkObject::Init()
{
}

void TalkObject::Update()
{
	(this->*m_update)();
}

void TalkObject::Draw()
{
	(this->*m_draw)();
}

void TalkObject::CanWeJustTalk()
{

}

void TalkObject::WaitingUpdate()
{
	if (m_canTalk)
	{

		
	}
}

void TalkObject::NormalDraw()
{
	DrawSphere3D(m_rigid->GetPos().VGet(), kRadius, 7, 0x00ff00, 0xffffff, true);
	DrawSphere3D(m_rigid->GetPos().VGet(), kTalkingRadius, 7, 0x00ff00, 0xffffff, false);
}

void TalkObject::OnTriggerEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		UI::GetInstance().WannaTalk(std::dynamic_pointer_cast<TalkObject>(shared_from_this()));
		m_canTalk = true;

	}
}

void TalkObject::OnTriggerStay(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		
		
	}
}

void TalkObject::OnTriggerExit(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		UI::GetInstance().TalkExit();
		m_update = &TalkObject::WaitingUpdate;
	}
}
