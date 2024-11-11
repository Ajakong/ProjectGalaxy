#pragma once
#include "Collidable.h"
#include"Vec3.h"
#include"Quaternion.h"
#include"ColliderSphere.h"
#include<string>

class Camera;
class PlayerSphere;

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
	void SetVelocity(Vec3 pos) { m_rigid->SetVelocity(pos); }
	void AddVelocity(Vec3 pos) { m_rigid->AddVelocity(pos); }
	void SetPos(Vec3 pos) { m_rigid->SetPos(pos); }
	float GetHp() { return m_Hp; }
	void SetCameraToPlayer(Vec3 cameraToPlayer);
	Vec3 GetMoveDir() const{ return m_moveDir; }
	Vec3 GetNowPlanetPos() { return m_nowPlanetPos; }
	Vec3 GetNormVec() { return Vec3(m_rigid->GetPos() - m_nowPlanetPos).GetNormalized(); }
	Vec3 GetFrontVec() { return m_frontVec.GetNormalized(); }
	Vec3 GetSideVec() { return m_sideVec.GetNormalized(); }
	Vec3 GetShotDir() { return m_shotDir; }
	Vec3 GetLookPoint() { return m_lookPoint; }
	float GetRegenerationRange() { return m_regeneRange; }
	float GetCameraEasingSpeed() { return m_cameraEasingSpeed; }
	int WatchHp()const { return static_cast<int>(m_Hp); }
	bool GetOperationFlag()const { return m_isOperationFlag; }
	bool GetBoostFlag() const{ return m_isBoostFlag; }
	bool OnAiming() { return m_isAimFlag; }

	void SetBoost();
	void SetIsOperation(bool flag);
	void SetCameraAngle(float cameraAngle);
	void SetSideVec(Vec3 right) { m_sideVec = right; }
	void SetFrontVec(Vec3 front) { m_frontVec = front; }
	void SetUpVec(Vec3 up) { m_upVec = up.GetNormalized(); }
	void IsWarp() { m_isJumpFlag = true;}
	int GetPlayerModelhandle() { return m_modelHandle; }
	bool IsSearch() { return m_isSearchFlag; }
	bool OnDamage() { return m_isOnDamageFlag; }
	bool IsClear() { return m_isClearFlag; }
	int GetDamageFrame() { return m_damageFrame; }
	int& SetReverse() { return m_reverseFlag; }
	//int GetSearchRemainTime() { return m_searchRemainTime; }
	bool GetJumpFlag() { return m_isJumpFlag; }
	std::string GetState() { return m_state; }


	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider,MyEngine::ColliderBase::ColideTag tag);
	virtual void OnCollideStay(std::shared_ptr<Collidable> colider,MyEngine::ColliderBase::ColideTag tag);
	//メンバ関数ポインタ
	using playerState_t = void(Player::*)();
	playerState_t m_playerUpdate;

	playerState_t m_prevUpdate;
	using cameraState_t = void(Player::*)();
	cameraState_t m_cameraUpdate;

	void CommandJump();
	void BoostUpdate();
	void OperationUpdate();
private:
	Vec3 Move();

	void ShotTheStar();

	//アニメーションの進行
	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);
	//アニメーションの変更
	void ChangeAnim(int animIndex,int speed=1.f);

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
	void DashUpdate();

	void SpiningUpdate();
	void JumpingSpinUpdate();
	void JumpingUpdate();
	void DropAttackUpdate();
	void AimingUpdate();
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

	void SetShotDir();
	void DeleteManage();
	
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
	std::shared_ptr<MyEngine::ColliderSphere> m_headCol;
	std::shared_ptr<MyEngine::ColliderSphere> m_spinCol;
	std::shared_ptr<MyEngine::ColliderSphere> m_footCol;
	std::shared_ptr<MyEngine::ColliderSphere> m_bodyCol;
	float m_Hp;
	float m_cameraEasingSpeed;
	int m_modelHandle = 0;
	int m_aimGraphHandle = 0;

	int rotRad = 0;

	int m_speed = 1.f;

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

	std::list<std::shared_ptr<PlayerSphere>> m_sphere;
	
	std::string m_state;

	bool m_isOnDamageFlag;
	bool m_isSpinFlag;

	float m_regeneRange;
	float m_angle;
	float m_modelDirAngle;
	float m_spinAngle;
	float m_radius = 0;
	float m_attackRadius;

	Quaternion m_myQ;
	Vec3 m_cameraToPlayer;
	Vec3 m_cameraPos;
	Vec3 m_lookPoint;
	//std::shared_ptr<Camera> m_camera;
	Vec3 m_moveDir;
	Vec3 m_postMoveDir;
	Vec3 m_nowPlanetPos;
	Vec3 nowVec;
	Vec3 m_inputVec;

	Vec3 m_frontVec;
	Vec3 m_sideVec;
	Vec3 m_upVec;
	Vec3 m_postUpVec;

	Vec3 m_shotDir;
	Vec3 m_modelBodyRotate;
	//0.0f:prevが再生
	//1.0:currentが再生
	int m_currentAnimNo;//現在のアニメーション
	int m_prevAnimNo;//変更前のアニメーション
	float m_animBlendRate;//アニメーションの合成割合
	
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

	/*int m_searchRemainTime;
	int m_chargeRemainTime;*/

	float m_playAnimTime = 0.0f;
	float m_animTime_move = 0.0f;

	float m_playerRotateY = 0;
	float m_cameraAngle = 0;

	bool m_isVisibleFlag = false;
	bool m_isJumpFlag = false;
	bool m_isBoostFlag = false;
	bool m_isOperationFlag = false;
	bool m_isSearchFlag = false;
	bool m_isAimFlag = false;
	bool m_isClearFlag=false;

	int m_visibleCount = 0;

	int HitCount = 0;
};

