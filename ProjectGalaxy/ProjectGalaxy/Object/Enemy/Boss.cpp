#include "Boss.h"
namespace
{
	constexpr float kBodyRadiusSize = 150.f;
}
Boss::Boss():Enemy(Priority::Boss,ObjectTag::Enemy)
{

}

Boss::~Boss()
{
}

void Boss::Init()
{
}

void Boss::Update()
{
}

void Boss::Draw()
{
	DrawSphere3D(m_rigid->GetPos().VGet(), kBodyRadiusSize);
}
