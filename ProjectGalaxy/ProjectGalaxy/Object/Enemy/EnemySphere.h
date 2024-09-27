#pragma once
#include "../MyLib/Physics/Collidable.h"

class Enemy;
//
//class EnemySphere : public MyEngine::Collidable
//{
//public:
//	EnemySphere(Priority priority, ObjectTag tag);
//	virtual ~EnemySphere();
//
//	void Init();
//	void Update();
//	void Draw();
//
//private:
//	int m_color;
//	bool m_deleteFlag;
//
//	float m_radius = 0;
//
//	Vec3 m_velocity;
//	std::shared_ptr<Enemy>m_enemy;
//};

/// <summary>
/// Enemyが攻撃の際に生成する球体オブジェクト
/// </summary>
class EnemySphere : public MyEngine::Collidable
{
public:
	EnemySphere(MyEngine::Collidable::Priority priority, ObjectTag tag,std::shared_ptr<MyEngine::Collidable>enemy, Vec3 pos, Vec3 velocity, int moveNum, int color = 0xff0000);
	virtual ~EnemySphere();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	virtual void Hit();

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);
	virtual bool IsDelete() { return false; }
	void SetCounterFlag() { m_counterFlag = true; }
	Vec3 GetVelocity() { return m_velocity; }
	void SetVelocity(Vec3 vel) { m_velocity = vel; }
	void DeleteFlag() { m_isDeleteFlag = true; }
	bool GetCounterFlag() { return m_counterFlag; }
	//メンバ関数ポインタ
	using MoveState_t = void(EnemySphere::*)();
	MoveState_t m_moveUpdate;

protected:
	virtual void  StraightUpdate();//球を直線状に飛ばす

protected:


	int m_color = 0;
	bool m_isDeleteFlag = 0;
	bool m_counterFlag;

	float m_radius = 0;

	Vec3 m_velocity;
	std::shared_ptr<Enemy>m_enemy;

private:
	void DeleteJudge();

private:

};