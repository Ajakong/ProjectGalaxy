#include "Crystal.h"
#include"ColliderBox.h"

using namespace std;


Crystal::Crystal(Vec3 pos,Vec3 norm,Vec3 size) : Collidable(Priority::StageGimmick,ObjectTag::Crystal)
{
	SetAntiGravity();
	m_size = size;
	m_rigid->SetPos(pos);
	auto col=AddCollider(MyEngine::ColliderBase::Kind::Box);
	auto box = dynamic_pointer_cast<MyEngine::ColliderBox>(col);
	box->norm = norm;
	box->size = m_size;
	box->rotation = Quaternion();
}

Crystal::~Crystal()
{
}

void Crystal::Init()
{
}

void Crystal::Update()
{
}

void Crystal::Draw()
{
	DrawCube3D((m_rigid->GetPos() - m_size).VGet(), (m_rigid->GetPos() + m_size).VGet(), 0x00ffff, 0xffffff, true);
	DrawCube3D((m_rigid->GetPos() - m_size).VGet(), (m_rigid->GetPos() + m_size).VGet(), 0xffffff, 0xffffff, false);
}
