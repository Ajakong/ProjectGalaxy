﻿#include "Coin.h"

Coin::Coin(Vec3 pos, bool antiGravity) : Item(pos,antiGravity)
{
}

Coin::~Coin()
{
}

void Coin::Init()
{
	SetObjectTag(ObjectTag::Coin);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	m_col = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	m_col->radius = 2.5f;
	m_col->isTrigger = true;
}

void Coin::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		m_isDestroyFlag = true;
	}
}