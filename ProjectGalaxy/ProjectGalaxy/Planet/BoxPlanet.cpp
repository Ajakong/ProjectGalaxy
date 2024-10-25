#include "BoxPlanet.h"
#include"ColliderBox.h"

BoxPlanet::BoxPlanet(Vec3 pos, int color)
{
	m_color = color;
	gravityPower = 3;
	{
		AddCollider(MyEngine::ColliderBase::Kind::Box);//ここで入れたのは重力の影響範囲
		m_colliders.back()->isTrigger = true;
		auto item = dynamic_pointer_cast<MyEngine::ColliderBox>(m_colliders.back());
		item->norm = Vec3(0, 1, 0);
		item->size = Vec3(300, 500, 500);
		item->rotation = Quaternion();
		norm = item->norm;
		size = item->size;
	}
	
	{
		AddCollider(MyEngine::ColliderBase::Kind::Box);//ここで入れたのは重力の影響範囲
		auto item = dynamic_pointer_cast<MyEngine::ColliderBox>(m_colliders.back());
		item->norm = Vec3(0, 1, 0);
		item->size = Vec3(300, 1000, 500);
		item->rotation = Quaternion();
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
	DrawCube3D((m_rigid->GetPos() - size).VGet(), (m_rigid->GetPos() + size).VGet(), 0x00ffff, 0xffffff, true);
	DrawCube3D((m_rigid->GetPos() - size).VGet(), (m_rigid->GetPos() + size).VGet(), 0xffffff, 0xffffff, false);
}

Vec3 BoxPlanet::GravityEffect(std::shared_ptr<Collidable> obj)
{
	Vec3 ans = obj->GetRigidbody()->GetVelocity() + norm * -5;
	return ans;
}

Vec3 BoxPlanet::GetNormVec(Vec3 pos)
{
	return norm;
}

void BoxPlanet::OnTriggerEnter(std::shared_ptr<Collidable> colider)
{
}

void BoxPlanet::OnTriggerExit(std::shared_ptr<Collidable> colider)
{
}
