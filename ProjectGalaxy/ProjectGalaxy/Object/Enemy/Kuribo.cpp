#include "Kuribo.h"
#include"ColliderSphere.h"
using namespace MyEngine;

namespace
{
	constexpr float kRadius = 50.f;
	constexpr int kChaseSpeed = 2.f;

}

Kuribo::Kuribo(MyEngine::Collidable::Priority priority, ObjectTag tag, Vec3 pos, int moveNum):Enemy(-1,priority,tag)
{
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kRadius;
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
}

void Kuribo::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		m_attackDir = colider->GetRigidbody()->GetPos()-m_rigid->GetPos();
		m_attackDir.Normalize();
	}
}

void Kuribo::ChaceUpdate()
{
	m_rigid->SetVelocity(m_attackDir * kChaseSpeed);
}
