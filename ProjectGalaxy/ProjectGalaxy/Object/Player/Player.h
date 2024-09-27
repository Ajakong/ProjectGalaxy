#pragma once
#include "MyLib/Physics/Collidable.h"
#include"MyLib/Vec3.h"

class Camera;

class Player : public MyEngine::Collidable
{
public:
	Player(int modelhandle);
	~Player();

	void Init();

	void Update();
	void SetMatrix();
	void Draw();

	Vec3 GetPos() const { return  m_rigid->GetPos(); }
	float GetHp() { return m_Hp; }
	void SetCameraToPlayer(Vec3 cameraToPlayer);
	Vec3 GetMoveDir() const{ return m_moveDir; }
	Vec3 GetNowPlanetPos() { return m_nowPlanetPos; }
	Vec3 GetNormVec() { return Vec3(m_rigid->GetPos() - m_nowPlanetPos).GetNormalized(); }
	float GetRegenerationRange() { return m_regeneRange; }
	int WatchHp()const { return static_cast<int>(m_Hp); }
	bool GetBoostFlag() { return m_isBoostFlag; }

	void SetBoost() { m_isBoostFlag = true; }
	void SetCameraAngle(float cameraAngle);
	void SetSideVec(Vec3 right) { m_sideVec = right; }
	void SetFrontVec(Vec3 front) { m_frontVec = front; }
	void SetUpVec(Vec3 up) { m_upVec = up; }
	void IsWarp() { m_isJumpFlag = true;}
	int GetPlayerModelhandle() { return m_modelHandle; }
	bool IsSearch() { return m_isSearchFlag; }
	bool OnDamage() { return m_isOnDamageFlag; }
	bool IsClear() { return m_isClearFlag; }
	int GetDamageFrame() { return m_damageFrame; }
	int& SetReverse() { return m_reverseFlag; }
	int GetSearchRemainTime() { return m_searchRemainTime; }
	bool GetJumpFlag() { return m_isJumpFlag; }

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);
	virtual void OnCollideStay(std::shared_ptr<Collidable> colider);
	//メンバ関数ポインタ
	using playerState_t = void(Player::*)();
	playerState_t m_playerUpdate;

	playerState_t m_prevUpdate;
	using cameraState_t = void(Player::*)();
	cameraState_t m_cameraUpdate;

	void BoostUpdate();
private:
	//アニメーションの進行
	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);
	//アニメーションの変更
	void ChangeAnim(int animIndex);

	//状態別関数(ポインタで呼び出す)
	/*m_playerUpdateで使う*/
	/// <summary>
	/// 開始直後に呼ばれる
	/// </summary>
	void StartUpdate();
	/// <summary>
	/// 通常時
	/// </summary>
	void NeutralUpdate();
	void WalkingUpdate();
	void SpiningUpdate();
	void JumpingSpinUpdate();
	void JumpingUpdate();
	/// <summary>
	/// ダメージ時
	/// </summary>
	void DamegeUpdate();
	/// <summary>
	/// 回避
	/// </summary>
	void AvoidUpdate();
	/*m_cameraUpdateで使う*/
	void Planet1Update();

	
	Vec3 GetCameraToPlayer()const;

private:
	struct UserData
	{
		float dissolveY;	// ディゾルヴしたい高さ
		float minY;
		float maxY;
		float dummy;
		float clickedU;
		float clickedV;
		float dummy2[2];
	};

	float m_Hp;
	int m_modelHandle = 0;

	int rotRad = 0;

	int m_itemCount=0;

	/// <summary>
	/// 行動のフレームを管理する
	/// </summary>
	int actionFrame = 0;
	int m_pointLightHandle = -1;
	int m_hitSEHandle;
	int m_parrySEHandle;
	int m_searchSEHandle;
	int m_getItemHandle;
	int m_color;
	int m_spinCount;
	Vec3 m_postUpVec;

	bool m_isOnDamageFlag;
	bool m_isSpinFlag;

	float m_regeneRange;
	float m_angle;
	float m_spinAngle;
	float m_radius = 0;
	float m_attackRadius;

	Vec3 m_cameraToPlayer;
	Vec3 m_cameraPos;
	//std::shared_ptr<Camera> m_camera;
	Vec3 m_moveDir;
	Vec3 m_nowPlanetPos;

	Vec3 m_frontVec;
	Vec3 m_sideVec;
	Vec3 m_upVec;

	int m_currentAnimNo;//現在のアニメーション
	int m_prevAnimNo;//変更前のアニメーション
	float m_animBlendRate;//アニメーションの合成割合
	//0.0f:prevが再生
	//1.0:currentが再生
	int m_reverseFlag=0;
	int m_damageFrame;
	int m_damageFrameSpeed;

	//アニメーション変数
	int m_anim_nutral = 0;
	int m_anim_move = 0;
	int m_anim_hit = 0;
	int m_anim_jump = 0;
	int m_attach_move = 0;
	int m_attach_hit = 0;
	int m_attach_jump = 0;
	int m_attach_nutral = 0;

	int m_searchRemainTime;
	int m_chargeRemainTime;

	float m_playAnimTime = 0.0f;
	float m_animTime_move = 0.0f;

	float m_playerRotateY = 0;
	float m_cameraAngle = 0;

	bool m_isVisibleFlag = false;
	bool m_isJumpFlag = false;
	bool m_isBoostFlag = false;
	bool m_isSearchFlag = false;
	bool m_isClearFlag=false;

	int m_visibleCount = 0;

	int HitCount = 0;
};

