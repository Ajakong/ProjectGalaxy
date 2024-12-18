﻿#pragma once
#include "Enemy.h"
#include"EnemySphere.h"

class Player;
class Item;

/// <summary>
/// 左右に移動し、一定間隔で射撃してくる敵e
/// 踏みつけられるとつぶれる
/// </summary>
class Takobo : public Enemy
{
public:
	enum AnimNum : int
	{
		Empty1,
		Brow,
		FastWalk,
		Idle,
		Empty2,
		Shot
	};

	/// <param name="m_modelhandle">エネミーのモデル</param>
	/// <param name="obj">影響を与えるオブジェ</param>
	Takobo(Vec3 pos, std::shared_ptr<MyEngine::Collidable> target);
	virtual ~Takobo();

	void Init();
	void Update();
	void SetMatrix();
	void DeleteManage();
	void Draw();

	virtual void OnCollideEnter(std::shared_ptr <MyEngine::Collidable> colider, ColideTag ownTag, ColideTag targetTag);

	Vec3 GetPos() const;
	Vec3 GetUpVec() const;
	Vec3 GetFrontVec() const;
	Vec3 GetHeadPos()const;
	int WatchHp() const { return m_hp; }
	float GetColRadius() { return m_radius; }

	float GetIdleSpeed() { return m_idleSpeed; }

	void SetTarget(std::shared_ptr<MyEngine::Collidable> target);
	void SetNormVec(Vec3 norm) { m_normVec = norm; }
	Vec3 GetShotUpVec() { return m_shotUpVec; }
	std::list<std::shared_ptr<EnemySphere>> GetAttackObj() { return m_sphere; }
	

	//メンバ関数ポインタ
	using enemyState_t = void(Takobo::*)();
	enemyState_t m_enemyUpdate;
private:
	//アニメーションの進行
	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);
	//アニメーションの変更
	void ChangeAnim(int animIndex, int speed = 1.f);


	/// <summary>
	/// 通常(クールダウン)状態
	/// </summary>
	void IdleUpdate();

	void DeathUpdate();
	/// <summary>
	/// 球体を生成して攻撃
	/// </summary>
	void AttackSphereUpdate();

	Vec3 GetAttackDir() const;
private:
	int m_hp;

	int m_attackCoolDownCount;

	float m_radius = 0;

	int m_modelHandle = 0;
	int m_modelHeadIndex;

	int m_createFrameCount = 0;

	float m_centerToEnemyAngle;

	float m_idleSpeed = 0;

	int m_sphereNum = 0;
	int m_bombNum = 0;
	int m_shotSEHandle;

	int m_animationSpeed;

	//0.0f:prevが再生
	//1.0:currentが再生
	int m_currentAnimNo;//現在のアニメーション
	int m_prevAnimNo;//変更前のアニメーション
	float m_animBlendRate;//アニメーションの合成割合

	Vec3 m_vec;
	Vec3 m_attackDir;
	Vec3 m_moveShaftPos;
	Vec3 m_normVec;
	/// <summary>
	/// 撃った弾の上方向ベクトル
	/// </summary>
	Vec3 m_shotUpVec;
	Vec3 m_nowPlanetPos;
	Vec3 m_strikePoint;
	std::shared_ptr<MyEngine::Collidable> m_target;
	std::shared_ptr<Item>m_dropItem;

	std::list<std::shared_ptr<EnemySphere>> m_sphere;
};

