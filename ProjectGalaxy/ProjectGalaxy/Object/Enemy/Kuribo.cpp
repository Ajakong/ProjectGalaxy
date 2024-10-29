#include "Kuribo.h"
#include"ColliderSphere.h"
using namespace MyEngine;

namespace
{
	constexpr float kRadius = 50.f;
	constexpr int kSearchRadius = 200.f;
	constexpr int kChaseSpeed = 2.f;

	constexpr int kChaseMaxFrame = 200;

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
	SetDrawBlendMode(DX_BLENDMODE_MULA, 100);
	DrawSphere3D(m_rigid->GetPos().VGet(), kRadius, 8, 0xffaa00, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#ifdef _DEBUG
	SetDrawBlendMode(DX_BLENDMODE_MULA, 100);
	DrawSphere3D(m_rigid->GetPos().VGet(), kSearchRadius, 8, 0xffaa00, 0xffffff, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
#endif
	DrawLine3D(m_rigid->GetPos().VGet(), (m_rigid->GetPos() + m_attackDir * 300).VGet(), 0x00ff00);
	DrawLine3D(m_rigid->GetPos().VGet(), (m_rigid->GetPos() + m_upVec * 300).VGet(), 0xff0000);
}

void Kuribo::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
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
		}
	}
}

void Kuribo::SearchUpdate()
{
	m_rigid->SetVelocity(Vec3(0, 0, 0));
	if (!m_player.get())return;
	m_rigid->SetVelocity(m_upVec * 10);
	m_moveUpdate = &Kuribo::JumpUpdate;
}

void Kuribo::JumpUpdate()
{
	
	if (m_rigid->GetVelocity().Length() == 0)
	{
		m_moveUpdate = &Kuribo::ChaseUpdate;
	}
}

void Kuribo::ChaseUpdate()
{
	m_chaseFrameCount++;
	m_rigid->SetVelocity(m_attackDir * kChaseSpeed);
	if (m_chaseFrameCount > kChaseMaxFrame)
	{
		m_chaseFrameCount = 0;
		m_moveUpdate = &Kuribo::SearchUpdate;
		m_player = nullptr;
	}
}
