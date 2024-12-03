#include "SamuraiAlter.h"
#include"ModelManager.h"

namespace
{
	const char* kModelFileName = "AlienSoldier.mv1";
}

SamuraiAlter::SamuraiAlter() : Enemy(Priority::High,ObjectTag::Enemy)
{
	m_modelHandle = ModelManager::GetInstance().GetModelData(kModelFileName);
}

SamuraiAlter::~SamuraiAlter()
{
}

void SamuraiAlter::Init()
{
}

void SamuraiAlter::Update()
{
}

void SamuraiAlter::SetMatrix()
{
	MV1SetRotationZYAxis(m_modelHandle, (m_moveDir * -1).VGet(), m_upVec.GetNormalized().VGet(), 0);
	MV1SetPosition(m_modelHandle, m_rigid->GetPos().VGet());
}

void SamuraiAlter::Draw()
{
	MV1DrawModel(m_modelHandle);
}
