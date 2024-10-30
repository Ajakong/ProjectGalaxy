#include "Kuribo.h"
#include"ColliderSphere.h"
using namespace MyEngine;

namespace
{
	constexpr float kRadius = 50.f;
	constexpr float kSearchRadius = 200.f;
	constexpr float kChaseSpeed = 0.5f;

	constexpr int kChaseMaxFrame = 200;

	constexpr float kAwayStrength = 6.f;

}

float GetVec2Angle(Vec3 a, Vec3 b)
{
	float cos = Dot(a.GetNormalized(), b.GetNormalized());//ない席は180度まで
	float rad = acos(cos);

#ifdef _DEBUG
	DrawFormatString(0, 125, 0xffffff, "rad(%f),deg(%f)", rad, rad * 180 / DX_PI_F);
#endif

	return rad;
}

Kuribo::Kuribo(Vec3 pos, int moveNum):Enemy(-1,Priority::Low,ObjectTag::Kuribo),
m_attackDir(0,0,1),
m_chaseFrameCount(0)
{
	m_comebackPoint = pos;
	m_rigid->SetPos(pos);
	{
		AddCollider(MyEngine::ColliderBase::Kind::Sphere);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		item->radius = kRadius;
	}
	
#ifdef _DEBUG
	{
		AddCollider(MyEngine::ColliderBase::Kind::Sphere);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		item->radius = kSearchRadius;
		item->isTrigger = true;
	}
#endif
	m_moveUpdate = &Kuribo::SearchUpdate;
}

Kuribo::~Kuribo()
{
}

void Kuribo::Init()
{
}

void Kuribo::Update()
{
	(this->*m_moveUpdate)();
}

void Kuribo::Draw()
{
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());

	SetDrawBlendMode(DX_BLENDMODE_MULA, 100);
	DrawSphere3D(m_rigid->GetPos().VGet(), kRadius, 8, 0xffaa00, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#ifdef _DEBUG
	SetDrawBlendMode(DX_BLENDMODE_MULA, 100);
	DrawSphere3D(m_rigid->GetPos().VGet(), item->radius, 8, 0xffaa00, 0xffffff, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#endif
	DrawLine3D(m_rigid->GetPos().VGet(), (m_rigid->GetPos() + m_attackDir * 300).VGet(), 0x00ff00);
	DrawLine3D(m_rigid->GetPos().VGet(), (m_rigid->GetPos() + m_upVec * 300).VGet(), 0xff0000);
}

void Kuribo::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Stage)
	{
		if (m_moveUpdate == &Kuribo::JumpUpdate)
		{
			m_moveUpdate = &Kuribo::DeathUpdate;
		}
	}
	if (colider->GetTag() == ObjectTag::Player)
	{
		m_rigid->SetVelocity(m_attackDir * -kAwayStrength + m_upVec * kAwayStrength*1.5f);
		m_moveUpdate = &Kuribo::JumpUpdate;
	}
}

void Kuribo::OnTriggerStay(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		Vec3 ToVec = colider->GetRigidbody()->GetPos() - m_rigid->GetPos();
		ToVec.Normalize();
		if (GetVec2Angle(m_attackDir, ToVec) <= DX_PI_F /2)
		{
			m_attackDir = colider->GetRigidbody()->GetPos() - m_rigid->GetPos();
			m_attackDir.Normalize();
			m_player = colider;

			m_targetPoint = colider->GetRigidbody()->GetPos();
		}
	}
}

void Kuribo::SearchUpdate()
{
	m_rigid->SetVelocity(Vec3(0, 0, 0));
	if (!m_player.get())return;
	m_rigid->SetVelocity(m_upVec * 10);
	
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kSearchRadius * 2;
	m_comebackPoint = m_rigid->GetPos();
	m_moveUpdate = &Kuribo::ChaseUpdate;

}

void Kuribo::JumpUpdate()
{
	m_initTime++;
	if (m_initTime >= 60)
	{
		m_isDestroyFlag = true;
	}
}

void Kuribo::ChaseUpdate()
{
	
	if (!m_player.get())m_chaseFrameCount++;
	m_attackDir = m_targetPoint - m_rigid->GetPos();
	m_attackDir.Normalize();
	m_rigid->SetVelocity(m_attackDir * kChaseSpeed);
	if (m_chaseFrameCount > kChaseMaxFrame)
	{
		m_chaseFrameCount = 0;
		
		m_moveUpdate = &Kuribo::ComebackUpdate;
		m_player = nullptr;
	}
}

void Kuribo::ComebackUpdate()
{
	Vec3 vec = m_comebackPoint - m_rigid->GetPos();
	vec.Normalize();
	m_rigid->SetVelocity(vec*kChaseSpeed);
	if ((m_comebackPoint - m_rigid->GetPos()).Length() <= 30)
	{
		m_moveUpdate = &Kuribo::SearchUpdate;
	}
	if (!m_player.get())
	{
		m_attackDir = vec;
		return;
	}
	m_rigid->SetVelocity(m_upVec * 10);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kSearchRadius;
	m_comebackPoint = m_rigid->GetPos();
	m_moveUpdate = &Kuribo::ChaseUpdate;
}

void Kuribo::DeathUpdate()
{
	m_rigid->SetVelocity(Vec3::Zero());
}
