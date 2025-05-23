﻿#include "ClearObject.h"
#include"../MyLib/Physics/ColliderSphere.h"
#include"../Quaternion.h"

ClearObject::ClearObject(Vec3 pos,bool antigravity) : Item(pos,ObjectTag::ClearObject,antigravity)
{

}

ClearObject::~ClearObject()
{
}

void ClearObject::OnCollideEnter(std::shared_ptr<Collidable> colider,ColideTag ownTag,ColideTag targetTag)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		m_isDestroy = true;
	}
}

void ClearObject::Init()
{
	SetObjectTag(ObjectTag::ClearObject);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere,ColideTag::Body);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back()->col);
	item->radius = 5;
	item->m_isTrigger = true;
}

void ClearObject::Draw()
{
	Vec3 zero = { 0,0,0 };
	m_rigid->SetVelocity(Vec3(0, 0, 0));
	Vec3 offSetVec = GetCameraRightVector();
	offSetVec -= GetCameraUpVector();
	offSetVec *= 9/5;
	Quaternion myQ;
	m_angle += 0.05f;

	Vec3 front = GetCameraFrontVector();
	for (int i = 0; i < 3; i++)
	{
		myQ.SetQuaternion(offSetVec);
		myQ.SetMove(DX_PI_F * 2 / 3 * i + m_angle, front);
		Vec3 offSet = myQ.Move(offSetVec, zero);
		DrawSphere3D((m_rigid->GetPos() + offSet).VGet(), 1, 8, 0xffffff, 0xff00ff, false);
	}
	for (int i = 0; i < 6; i++)
	{
		myQ.SetQuaternion(offSetVec*2);
		myQ.SetMove(DX_PI_F * 1 / 3 * i + m_angle, front);
		Vec3 offSet = myQ.Move(offSetVec, zero);
		DrawSphere3D((m_rigid->GetPos() + offSet).VGet(), 1, 8, 0x0000ff, 0xff00ff, false);
	}

	DrawSphere3D(m_rigid->GetPos().VGet(), 5, 8, 0x00ff00, 0xffff00, false);
}
