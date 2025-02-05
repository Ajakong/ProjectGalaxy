﻿#pragma once
#include "Enemy.h"
#include"ColliderSphere.h"
#include"StampImpact.h"
#include"ClearObject.h"

class Planet;
class Player;
class Killer;

class Boss : public Enemy
{
public:
	Boss(Vec3 pos,std::shared_ptr<Player>player);
	virtual ~Boss();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

	void WakeUp() { m_isWakeUp = true; }
private:

	/// <summary>
	/// 第1フェーズの行動
	/// </summary>
	void PhaseOneUpdate();
	/// <summary>
	/// 第2フェーズの行動
	/// </summary>
	void PhaseTwoUpdate();
	/// <summary>
	/// 第3フェーズの行動
	/// </summary>
	void PhaseThreeUpdate();


	void InitUpdate();
	/// <summary>
	/// 休憩
	/// </summary>
	void RestUpdate();
	void NeutralUpdate();
	/// <summary>
	/// 怒り
	/// </summary>
	void AnglyUpdate();
	/// <summary>
	/// 惑星破壊
	/// </summary>
	void DestroyPlanetUpdate();
	/// <summary>
	/// のけぞり
	/// </summary>
	void KnockBackUpdate();
	/// <summary>
	/// 衝撃波
	/// </summary>
	void JumpingUpdate();
	/// <summary>
	/// フルパワー衝撃波
	/// </summary>
	void FullpowerJumpUpdate();
	//第2フェーズで追加される攻撃
	
	/// <summary>
	/// 突進
	/// </summary>
	void TackleUpdate();
	/// <summary>
	/// 直進
	/// </summary>
	void RunningUpdate();
	/// <summary>
	/// 着地
	/// </summary>
	void LandingUpdate();

	/// <summary>
	/// 惑星移動
	/// </summary>
	void RunawayUpdate();

	void BallAttackUpdate();

	//アニメーションの進行
	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);
	//アニメーションの変更
	void ChangeAnim(int animIndex, int speed = 1.f);

	// 衝突したとき
	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag);
	virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag);

	template <typename T>
	void DeleteObject(std::vector<std::shared_ptr<T>>& objects);
private:
	//メンバ関数ポインタ
	using bossState_t = void(Boss::*)();
	bossState_t m_bossUpdate;
	bossState_t m_phaseUpdate;

	Vec3 m_runningDir;
	Vec3 m_runawayPos;

	int m_animationSpeed;

	//0.0f:prevが再生
	//1.0:currentが再生
	int m_currentAnimNo;//現在のアニメーション
	int m_prevAnimNo;//変更前のアニメーション
	float m_animBlendRate;//アニメーションの合成割合
	int m_knockBackFrame;
	int m_jumpCount;
	
	int m_damageSoundHandle;
	int m_criticalHandle;
	int m_dropSoundHandle;

	/// <summary>
	/// マイナスの時はひるみ、もしくは動けない状態。
	/// </summary>
	int m_actionFrame;
	/// <summary>
	/// タックルするためのチャージ時間
	/// </summary>
	int m_tackleChargeFrame;
	/// <summary>
	/// 走り回ってる時間カウント
	/// </summary>
	int m_runningFrame;
	int m_color;

	int m_shotCreateFrame;
	

	bool m_isHit;
	bool m_onColStage;
	bool m_isWakeUp;
	bool m_isTalk;
	bool m_isTackle;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<Planet> m_nowPlanet;
	std::shared_ptr<MyEngine::ColliderSphere> m_collision;
	std::vector<std::shared_ptr<StampImpact>> m_impacts;
	std::vector<std::shared_ptr<Killer>> m_killer;
	std::shared_ptr<ClearObject>m_dropItem;
};

