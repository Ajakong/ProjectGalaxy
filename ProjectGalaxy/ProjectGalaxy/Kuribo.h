#pragma once
#include "Enemy.h"
class Kuribo : public Enemy
{
public:
	Kuribo(Vec3 pos, int moveNum);
	virtual ~Kuribo();

	void Init();
	void Update();
	void Draw();
	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);
	virtual void OnTriggerStay(std::shared_ptr<Collidable> colider);

	//メンバ関数ポインタ
	using MoveState_t = void(Kuribo::*)();
	MoveState_t m_moveUpdate;

private:
	void SearchUpdate();
	void JumpUpdate();
	void ChaseUpdate();

	Vec3 m_vec;
	Vec3 m_attackDir;
	Vec3 m_moveShaftPos;
	Vec3 m_normVec;
	Vec3 m_nowPlanetPos;

	std::shared_ptr<Collidable> m_player;

	int m_chaseFrameCount;
	int m_initTime;
};

