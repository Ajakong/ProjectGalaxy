#include "BoxPlanet.h"
#include"MyLib/Physics/ColliderBox.h"

BoxPlanet::BoxPlanet(Vec3 pos, int color)
{
	m_color = color;
	gravityPower = 3;
	{
		AddCollider(MyEngine::ColliderBase::Kind::Box);//ここで入れたのは重力の影響範囲
		m_colliders.back()->isTrigger = true;
		auto item = dynamic_pointer_cast<MyEngine::ColliderBox>(m_colliders.back());
		item->m_startPos = pos;
	}
	
	{
		AddCollider(MyEngine::ColliderBase::Kind::Box);//ここで入れたのは重力の影響範囲
		auto item = dynamic_pointer_cast<MyEngine::ColliderBox>(m_colliders.back());

	}
	AddThroughTag(ObjectTag::Stage);
	AddCollider(MyEngine::ColliderBase::Kind::Box);//マップの当たり判定
	m_rigid->SetPos(pos);

	m_pointLightHandle = CreatePointLightHandle(m_rigid->GetPos().VGet(), 5000.0f, 0.0f, 0.002f, 0.0f);
}

BoxPlanet::~BoxPlanet()
{
}

void BoxPlanet::Init()
{
}

void BoxPlanet::Update()
{
}

void BoxPlanet::Draw()
{
}

void BoxPlanet::GravityEffect()
{
}

Vec3 BoxPlanet::GetNormVec(Vec3 pos)
{
	return Vec3(0,1,0);
}

void BoxPlanet::OnTriggerEnter(std::shared_ptr<Collidable> colider)
{
}

void BoxPlanet::OnTriggerExit(std::shared_ptr<Collidable> colider)
{
}
