#pragma once
#include "Enemy.h"
#include"ColliderSphere.h"
#include"StampImpact.h"
#include"ClearObject.h"

class Planet;
class Player;
class Boss : public Enemy
{
public:
	Boss(Vec3 pos,std::shared_ptr<Player>player);
	virtual ~Boss();

	virtual void Init();
	virtual void Update();
	virtual void Draw();

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
	void RestUpdate();
	void NeutralUpdate();
	void AnglyUpdate();
	void DestroyPlanetUpdate();
	void KnockBackUpdate();
	void JumpingUpdate();
	void FullpowerJumpUpdate();
	void TackleUpdate();
	void RunningUpdate();
	void LandingUpdate();

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
	

	bool m_isHit;
	bool m_onColStage;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<Planet> m_nowPlanet;
	std::shared_ptr<MyEngine::ColliderSphere> m_collision;
	std::vector<std::shared_ptr<StampImpact>> m_impacts;
	std::shared_ptr<ClearObject>m_dropItem;
};

