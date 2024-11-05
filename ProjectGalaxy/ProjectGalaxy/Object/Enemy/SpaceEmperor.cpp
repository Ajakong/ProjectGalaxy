#include "SpaceEmperor.h"
#include"ModelManager.h"
#include"ColliderSphere.h"
namespace
{
	const char* kSpaceEmperorFileName = "SpaceEmperor.mv1";
}

SpaceEmperor::SpaceEmperor(Vec3 pos) : Enemy(-1,Priority::High,ObjectTag::SpaceEmperor)
{
	m_rigid->SetPos(pos);
	m_handle = ModelManager::GetInstance().GetModelData(kSpaceEmperorFileName);
	m_update = &SpaceEmperor::IntroUpdate;
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = 6;
	MV1SetScale(m_handle,Vec3(0.3f, 0.3f, 0.3f).VGet());
}

SpaceEmperor::~SpaceEmperor()
{
}

void SpaceEmperor::Init()
{
}

void SpaceEmperor::Update()
{
	(this->*m_update)();
}

void SpaceEmperor::SetMatrix()
{
	MV1SetPosition(m_handle,m_rigid->GetPos().VGet());
}

void SpaceEmperor::Draw()
{
	MV1DrawModel(m_handle);
}

void SpaceEmperor::IntroUpdate()
{
	m_update = &SpaceEmperor::IdleUpdate;
}

void SpaceEmperor::IdleUpdate()
{
	m_rigid->SetVelocity(Vec3::Zero());
}

void SpaceEmperor::AttackUpdate()
{
	
}

void SpaceEmperor::HitUpdate()
{
}
