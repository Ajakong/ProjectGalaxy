﻿#pragma once
#include "Enemy.h"

class Item;
/// <summary>
/// プレイヤーを視認後その方向に走り続ける敵
/// </summary>
class Gorori : public Enemy
{
public:
	/// <param name="m_modelhandle">エネミーのモデル</param>
		/// <param name="obj">影響を与えるオブジェ</param>
	Gorori(Vec3 pos,std::shared_ptr<Collidable>target);
	~Gorori();

	void Init();
	void Update();
	void SetMatrix();
	void Draw();

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider,ColideTag ownTag,ColideTag targetTag);
	virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag);
	Vec3 GetMyPos();
	int WatchHp() const { return m_hp; }
	float GetColRadius() { return m_radius; }

	float GetIdleSpeed() { return m_idleSpeed; }
	void SetAttackDir(Vec3 DirVec) { m_attackDir = DirVec; }

	void SetTarget(std::shared_ptr<Collidable> target);

private:
	/// <summary>
	/// 徘徊状態
	/// </summary>
	void IdleUpdate();
	/// <summary>
	/// 攻撃中
	/// </summary>
	void AttackUpdate();


	Vec3 GetAttackDir() const;
private:
	//メンバ関数ポインタ
	using enemyState_t = void(Gorori::*)();
	enemyState_t m_enemyUpdate;
	int m_hp;
	int m_color;
	int m_attackCoolDownCount;
	
	int m_modelHandle = 0;
	int m_createFrameCount = 0;
	int m_attackCount = 0;
	int m_sphereNum = 0;
	int m_bombNum = 0;

	float m_radius = 0;
	float m_idleSpeed = 0;
	float m_centerToEnemyAngle;

	Vec3 m_vec;
	Vec3 m_attackDir;
	Vec3 m_moveShaftPos;
	std::shared_ptr<Collidable> m_target;
	std::shared_ptr<Item>m_dropItem;
	//アニメーション変数
	int m_anim_nutral = 0;
	int m_anim_move = 0;
	int m_anim_jump = 0;
	int m_anim_attack = 0;
};

