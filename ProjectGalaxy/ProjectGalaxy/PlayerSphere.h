#pragma once
#include "SphereBase.h"
class Player;

class PlayerSphere : public SphereBase
{
public:
	PlayerSphere(MyEngine::Collidable::Priority priority, ObjectTag tag, std::shared_ptr<MyEngine::Collidable>player, Vec3 pos, Vec3 velocity,Vec3 sideVec, int moveNum, int color= 0xff0000);
	virtual ~PlayerSphere();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	virtual void Hit();

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider,MyEngine::ColliderBase::ColideTag tag);

	//メンバ関数ポインタ
	using MoveState_t = void(PlayerSphere::*)();
	MoveState_t m_moveUpdate;

	std::shared_ptr<Player>m_player;

protected:
	virtual void  StraightUpdate();//球を直線状に飛ばす

protected:
	
	Vec3 m_sideVec;
	int m_lifeTime;
};

