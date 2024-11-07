#pragma once
#include "Enemy.h"
#include"ColliderSphere.h"

using namespace MyEngine;
class Kuribo : public Enemy
{
public:
	Kuribo(Vec3 pos, int moveNum);
	virtual ~Kuribo();

	void Init();
	void Update();
	void SetMatrix();
	void Draw();
	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider,MyEngine::ColliderBase::ColideTag tag);
	virtual void OnTriggerStay(std::shared_ptr<Collidable> colider,MyEngine::ColliderBase::ColideTag tag);

	//メンバ関数ポインタ
	using MoveState_t = void(Kuribo::*)();
	MoveState_t m_moveUpdate;

private:
	void SearchUpdate();
	void JumpUpdate();
	void ChaseUpdate();
	void ComebackUpdate();
	void DeathUpdate();
	Vec3 m_vec;
	Vec3 m_attackDir;
	Vec3 m_moveShaftPos;
	Vec3 m_normVec;
	Vec3 m_nowPlanetPos;
	Vec3 m_comebackPoint;
	Vec3 m_targetPoint;
	std::shared_ptr<Collidable> m_player;
	std::shared_ptr<ColliderSphere> m_searchCol;

	int m_chaseFrameCount;
	int m_initTime;
};

