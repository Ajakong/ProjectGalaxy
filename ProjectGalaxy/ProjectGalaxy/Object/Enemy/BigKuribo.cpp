#include "BigKuribo.h"
#include"Physics.h"
#include"GalaxyCreater.h"

BigKuribo::BigKuribo(Vec3 pos,int connectNum) : Kuribo(pos)
{
	MV1SetScale(m_modelHandle, Vec3(0.1f, 0.1f, 0.1f).VGet());
	MV1SetPosition(m_modelHandle, pos.VGet());
	m_connectObjectNumber = connectNum;
}

BigKuribo::~BigKuribo()
{
	auto obj=GalaxyCreater::GetInstance().GetCollidable(m_connectObjectNumber);
	Physics::GetInstance().Entry(obj);
}
