#include "Player.h"
#include"Pad.h"
#include"ColliderSphere.h"
#include"Camera.h"
#include"SoundManager.h"
#include"Gorori.h"
#include"EnemySphere.h"
#include"PlayerSphere.h"
#include"KillerTheSeeker.h"
#include"ModelManager.h"
#include"GraphManager.h"

#include"Physics.h"

/// <summary>
/// やること:足の当たり判定を生成・踏みつけに使う
/// スピン専用の当たり判定を生成・体より半径が大きい当たり判定にし、スピン中にだけ出現
/// </summary>

namespace
{
	constexpr float kNetralRadius = 2;//通常時の当たり半径

	constexpr int kDamageFrameMax = 20;
	//アニメーション番号
	constexpr int kIdleAnimIndex = 1;
	//constexpr int kIdleAnimIndex = 2;//待機テスト
	constexpr int kAttackAnimIndex = 30;

	constexpr float kAnimFrameSpeed = 30.0f;//アニメーション進行速度

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//アナログスティックによる移動関連
	constexpr float kMaxSpeed = 0.5f;//プレイヤーの最大速度
	constexpr float kAnalogRangeMin = 0.1f;//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;//アナログスティックから入力されるベクトルの最大値

	constexpr float kFrameParSecond = 60.0f;

	constexpr int kAvoidFrame = 60;

	constexpr int kJumpPower = 1;

	constexpr int kSearchRemainTimeMax = 28;
	constexpr int kChargeRemainCoolTime = 10;

	const char* kGororiHitSEName = "GororiHitSE.mp3";
	const char* kGetItemSEName = "GetItemSE.mp3";
	const char* kOnParrySEName = "Parry.mp3";

	const char* kGetSearchSEName = "Search.mp3";
	const char* name = "Player";
	const char* kFileName = "SpaceHarrier.mv1";
	
	constexpr int kAnimationNumTpose = 0;
	constexpr int kAnimationNumHit = 1;
	constexpr int kAnimationNumJump = 2;
	constexpr int kAnimationNumRun = 3;
	constexpr int kAnimationNumSpin = 4;
	constexpr int kAnimationNumIdle = 5;
	constexpr int kAnimationNumDeath = 6;
	constexpr int kAnimationNumFall = 7;

	//照準
	const char* kAimGraphFileName = "Elements_pro.png";
	constexpr int kAimGraphSrkX = 3140;
	constexpr int kAimGraphSrkY = 200;
	constexpr int kAimGraphWidth = 400;
	constexpr int kAimGraphHeight = 370;
}

float GetAngle(Vec3 a, Vec3 b)
{
	a.Normalize();
	b.Normalize();

	float cos = Dot(a, b);
	// コサインをクリッピング
	cos = max(-1.0f, min(1.0f, cos));

	float rad = acos(cos);

#ifdef _DEBUG
	DrawFormatString(0, 125, 0xffffff, "rad(%f),deg(%f)", rad,rad*180/DX_PI_F);
#endif

	return rad;
}

void MTransCopy(MATRIX& in, const MATRIX& src) {
	in.m[3][0] = src.m[3][0]; in.m[3][1] = src.m[3][1]; in.m[3][2] = src.m[3][2]; in.m[3][3] = 1.f;
}


Player::Player(int modelhandle) : Collidable(Priority::High, ObjectTag::Player),
m_modelHandle(MV1DuplicateModel(ModelManager::GetInstance().GetModelData(kFileName))),
m_parrySEHandle(SoundManager::GetInstance().GetSoundData(kOnParrySEName)),
m_getItemHandle(SoundManager::GetInstance().GetSoundData(kGetItemSEName)),
m_searchSEHandle(SoundManager::GetInstance().GetSoundData(kGetSearchSEName)),
m_hitSEHandle(SoundManager::GetInstance().GetSoundData(kGororiHitSEName)),
m_aimGraphHandle(GraphManager::GetInstance().GetGraphData(kAimGraphFileName)),
m_upVec(Vec3::Up()),
m_postUpVec(Vec3::Up()),
m_nowPlanetPos(Vec3::Up() * 50),
m_shotDir(Vec3::Front()),
m_moveDir(Vec3::Front()),
m_postMoveDir(Vec3::Front()),
m_frontVec(Vec3::Front()),
m_playerUpdate(&Player::NeutralUpdate),
m_prevUpdate(&Player::NeutralUpdate),
m_cameraUpdate(&Player::Planet1Update),
m_anim_move(),
m_radius(kNetralRadius),
m_Hp(50),
m_damageFrame(0),
m_regeneRange(0),
m_angle(0),
m_spinAngle(0),
m_animBlendRate(0),
m_currentAnimNo(-1),
m_prevAnimNo(0),
m_isJumpFlag(false),
m_isSpinFlag(false),
m_isOperationFlag(false),
m_searchRemainTime(0),
m_chargeRemainTime(0),
m_color(0x00ffff),
m_attackRadius(0),
m_damageFrameSpeed(1),
m_modelBodyRotate(m_frontVec),
m_inputVec(0,0,-1),
m_modelDirAngle(0)
{
	m_postUpVec = m_upVec;
	{
		m_rigid->SetPos(Vec3(0, 0, 0));
		AddCollider(MyEngine::ColliderBase::Kind::Sphere, MyEngine::ColliderBase::ColideTag::Body);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		item->radius = m_radius;
	}

	{
		AddCollider(MyEngine::ColliderBase::Kind::Sphere, MyEngine::ColliderBase::ColideTag::Body);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		item->radius = m_attackRadius;
	}

	DxLib::MV1SetScale(m_modelHandle, VGet(0.005f, 0.005f, 0.005f));
	ChangeAnim(kAnimationNumIdle);	
	//m_prevUpdate = &Player::NeutralUpdate;
}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Init()
{
}

void Player::Update()
{
	m_isSearchFlag = false;
	m_sideVec = GetCameraRightVector();
	m_frontVec = Cross(m_sideVec, m_upVec).GetNormalized();
	(this->*m_playerUpdate)();
	m_chargeRemainTime++;
	if (Pad::IsTrigger(PAD_INPUT_Y))
	{
		if (m_isAimFlag)
		{
			m_isAimFlag = false;
		}
		else
		{
			m_isAimFlag = true;
		}
	}
	if ((Pad::IsTrigger(PAD_INPUT_Z)) && m_searchRemainTime >= 2)PlaySoundMem(m_searchSEHandle, DX_PLAYTYPE_BACK);
	if ((Pad::IsPress(PAD_INPUT_Z)))
	{
		m_isSearchFlag = true;
		m_searchRemainTime--;
		if (m_searchRemainTime < 0)m_searchRemainTime = 0;
		if (m_searchRemainTime <= 1)m_Hp -= 0.05f;
		if (m_Hp <= 0)m_Hp = 0.05f;
	}
	else
	{
		if (m_chargeRemainTime < kChargeRemainCoolTime)return;
		if (m_searchRemainTime <= kSearchRemainTimeMax)
		{
			m_chargeRemainTime = 0;
			m_searchRemainTime++;
		}
	}
	if (m_isAimFlag)
	{

		

	}

	SetShotDir();

	int index = MV1SearchFrame(m_modelHandle, "mixamorig:Spine");
	MATRIX shotDirMat = MGetRotVec2(nowVec.VGet(), m_shotDir.VGet());
	nowVec = m_shotDir.VGet();

	/*MATRIX localMat = MV1GetFrameLocalMatrix(m_modelHandle, index);
	MATRIX mat = MMult(shotDirMat, localMat);
	MV1SetFrameUserLocalMatrix(m_modelHandle, index, mat);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			DrawFormatString(i * 80, j * 16, GetColor(255, 255, 255), "%f", mat.m[i][j]);
		}
	}*/

	if (Pad::IsTrigger(PAD_INPUT_3))
	{
		ShotTheStar();
	}

	for (auto& item : m_sphere)
	{
		item->Update();
	}
	DeleteManage();
	
	if (m_visibleCount > 200)
	{
		m_isVisibleFlag = false;
		m_visibleCount = 0;
	}
	if (m_isVisibleFlag)
	{
		m_visibleCount++;
	}
	else
	{
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		item->radius = m_radius;	
	}
	if (m_isOnDamageFlag)
	{
		m_damageFrame--;
		if (m_damageFrame < 0)
		{
			m_damageFrame = 0;
			m_isOnDamageFlag = false;
		}

	}
	if (m_searchRemainTime <= 0)
	{
		m_damageFrame += m_damageFrameSpeed;
		if (m_damageFrame > 60)m_damageFrameSpeed *= -1;
		if (m_damageFrame < 0)m_damageFrameSpeed *= -1;
	}
	else
	{
		m_damageFrame = 0;
	}

	UpdateAnim(m_currentAnimNo);
	//変更前のアニメーション100%
	DxLib::MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	DxLib::MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
	m_animBlendRate += 0.05f;
	if (m_animBlendRate > 1.0f)
	{
		m_animBlendRate = 1.0f;
	}

	m_lookPoint = m_rigid->GetPos();
}

void Player::SetMatrix()
{
	Set3DSoundListenerPosAndFrontPosAndUpVec(m_rigid->GetPos().VGet(), Vec3(m_rigid->GetPos() + GetCameraFrontVector()).VGet(), m_upVec.VGet());

	//float angleY = atan2(-m_moveDir.x, -m_moveDir.z);
	//m_myQ = m_myQ * m_myQ.CreateRotationQuaternion(angleY -m_modelDirAngle, Vec3::Up());//角度の変化量だけ渡す
	//m_modelDirAngle = angleY;
	
	float angle = GetAngle(m_postUpVec, m_upVec);

	Vec3 axis = Cross(m_upVec, m_moveDir);
	axis.Normalize();
	m_angle +=angle;
	m_myQ =m_myQ*m_myQ.CreateRotationQuaternion(angle, axis);
	//m_myQ =m_myQ* m_myQ.CreateRotationQuaternion(atan2(-m_moveDir.x, -m_moveDir.z), m_upVec);
	auto rotatemat = m_myQ.ToMat();


	
	printf("x:%f,y:%f,z:%f\n", axis.x, axis.y, axis.z);
	
	
#ifdef _DEBUG

	DrawLine3D(m_rigid->GetPos().VGet(), Vec3(m_rigid->GetPos() + axis * 100).VGet(), 0xff00ff);
	DrawLine3D(m_rigid->GetPos().VGet(), Vec3(m_rigid->GetPos() + m_postUpVec * 100).VGet(), 0xaa0000);
	DrawLine3D(m_rigid->GetPos().VGet(), Vec3(m_rigid->GetPos() + m_moveDir * 100).VGet(), 0x00aa00);

#endif 

	MATRIX mat;
	
	mat = m_myQ.ToMat();

	m_postUpVec = m_upVec;
	
	MV1SetRotationMatrix(m_modelHandle, rotatemat);

	MV1SetPosition(m_modelHandle, m_rigid->GetPos().VGet());
	auto modelMat = MV1GetMatrix(m_modelHandle);
}

void Player::Draw()
{
	if (m_visibleCount % 5 == 0)
	{
		//MV1DrawModel(m_modelHandle);
	}
	MV1DrawModel(m_modelHandle);
	DrawSphere3D(m_rigid->GetPos().VGet(), m_radius, 10, m_color, 0xffffff, true);
	if (m_isSpinFlag)
	{
		DrawSphere3D(m_rigid->GetPos().VGet(), m_attackRadius, 10, 0x00ff00, 0xffffff, false);
	}

#if _DEBUG
	DrawLine3D(m_rigid->GetPos().VGet(), Vec3(m_rigid->GetPos() + m_shotDir * 100).VGet(), 0x0000ff);
	DrawLine3D(m_rigid->GetPos().VGet(), Vec3(m_rigid->GetPos() + m_sideVec * 100).VGet(), 0x00ff00);
	DrawLine3D(m_rigid->GetPos().VGet(), Vec3(m_rigid->GetPos() + m_upVec * 100).VGet(), 0xff0000);
	DrawLine3D(m_rigid->GetPos().VGet(), Vec3(m_rigid->GetPos() + m_moveDir * 100).VGet(), 0xffff00);
	//printfDx("%d", HitCount);
#endif
	if(m_isAimFlag)DrawRectRotaGraph(800, 450, kAimGraphSrkX, kAimGraphSrkY, kAimGraphWidth, kAimGraphHeight, 0.3, 0, m_aimGraphHandle, true);
}

void Player::SetCameraToPlayer(Vec3 cameraToPlayer)
{
	m_cameraToPlayer = cameraToPlayer;
}

void Player::SetBoost()
{
	m_isBoostFlag = true; 
	ChangeAnim(kAnimationNumFall);
}

void Player::SetIsOperation(bool flag)
{
	SetVelocity(Vec3::Zero());
	if (flag)
	{
		
		SetAntiGravity();
		m_playerUpdate = &Player::OperationUpdate;
		ChangeAnim(kAnimationNumFall);
		m_isOperationFlag = true;
	}
	else
	{
		SetAntiGravity(false);
		ChangeAnim(kAnimationNumIdle);
		m_isOperationFlag = false;
	}
}
void Player::SetCameraAngle(float cameraAngle)
{
	m_cameraAngle = cameraAngle;
}

void Player::OnCollideEnter(std::shared_ptr<Collidable> colider, MyEngine::ColliderBase::ColideTag tag)
{
	if (colider->GetTag() == ObjectTag::Coin)
	{
		m_Hp += 10;
	}

	if (colider->GetTag() == ObjectTag::Stage)
	{
		m_spinCount = 0;
		m_nowPlanetPos = colider->GetRigidbody()->GetPos();
		m_playerUpdate = &Player::NeutralUpdate;
		m_isJumpFlag = false;
		m_isBoostFlag = false;
		ChangeAnim(kAnimationNumIdle);
	}
	if (colider->GetTag() == ObjectTag::Crystal)
	{
		m_spinCount = 0;
		m_playerUpdate = &Player::NeutralUpdate;
		m_isJumpFlag = false;
		m_isBoostFlag = false;
		ChangeAnim(kAnimationNumIdle);
	}
	if (colider->GetTag() == ObjectTag::Kuribo)
	{
		if (m_isSpinFlag)
		{
			PlaySoundMem(m_parrySEHandle, DX_PLAYTYPE_BACK);
		}
		else
		{
			//HPを減らす

			//ノックバック
			Vec3 enemyAttackDir = m_rigid->GetPos() - colider->GetRigidbody()->GetPos();
			enemyAttackDir.Normalize();
			m_rigid->SetVelocity(enemyAttackDir * 2);
			m_playerUpdate = &Player::DamegeUpdate;
		}
	}
	if (colider->GetTag() == ObjectTag::Takobo)
	{
		if (m_isSpinFlag)
		{
			PlaySoundMem(m_parrySEHandle, DX_PLAYTYPE_BACK);
			colider->GetRigidbody()->SetVelocity(Vec3(m_rigid->GetPos() - colider->GetRigidbody()->GetPos()).GetNormalized() * -3);
		}
		else
		{
			PlaySoundMem(m_hitSEHandle, DX_PLAYTYPE_BACK);
			StartJoypadVibration(DX_INPUT_PAD1, 600, 600);
			m_Hp -= 10;
			m_prevUpdate = m_playerUpdate;
			m_playerUpdate = &Player::DamegeUpdate;
			m_rigid->AddVelocity(Vec3(m_rigid->GetPos() - colider->GetRigidbody()->GetPos()).GetNormalized() * 3);
			m_isOnDamageFlag = true;
			m_damageFrame = kDamageFrameMax;

			ChangeAnim(kAnimationNumHit);
		}
	}
	if (colider->GetTag() == ObjectTag::Gorori)
	{
		if (m_isSpinFlag)
		{
			PlaySoundMem(m_parrySEHandle, DX_PLAYTYPE_BACK);
			auto gorori = dynamic_pointer_cast<Gorori>(colider);

			gorori->SetAttackDir(Vec3(gorori->GetRigidbody()->GetPos() - m_rigid->GetPos()).GetNormalized());
		}
		else
		{
			PlaySoundMem(m_hitSEHandle, DX_PLAYTYPE_BACK);
			StartJoypadVibration(DX_INPUT_PAD1, 600, 600);
			m_Hp -= 10;
			m_prevUpdate = m_playerUpdate;
			m_playerUpdate = &Player::DamegeUpdate;
			m_rigid->AddVelocity(Vec3(m_rigid->GetPos() - colider->GetRigidbody()->GetPos()).GetNormalized() * 4);
			m_isOnDamageFlag = true;
			m_damageFrame = kDamageFrameMax;

			ChangeAnim(kAnimationNumHit);
		}
	}
	if (colider->GetTag() == ObjectTag::KillerTheSeeker)
	{
		if (m_isSpinFlag)
		{
			m_prevUpdate = m_playerUpdate;
			m_playerUpdate = &Player::DamegeUpdate;
			PlaySoundMem(m_parrySEHandle, DX_PLAYTYPE_BACK);
			auto killer = dynamic_pointer_cast<KillerTheSeeker>(colider);

			killer->SetVelocity(Vec3(killer->GetRigidbody()->GetPos() - m_rigid->GetPos()).GetNormalized());
			killer->m_Hp -= 20;
		}
		else
		{
			PlaySoundMem(m_hitSEHandle, DX_PLAYTYPE_BACK);
			StartJoypadVibration(DX_INPUT_PAD1, 600, 600);
			m_Hp -= 10;
			m_prevUpdate = m_playerUpdate;
			m_playerUpdate = &Player::DamegeUpdate;
			m_rigid->AddVelocity(Vec3(m_rigid->GetPos() - colider->GetRigidbody()->GetPos()).GetNormalized() * 4);
			m_isOnDamageFlag = true;
			m_damageFrame = kDamageFrameMax;
			ChangeAnim(kAnimationNumHit);
		}
	}
	if (colider->GetTag() == ObjectTag::Item)
	{
		PlaySoundMem(m_getItemHandle, DX_PLAYTYPE_BACK);
		m_itemCount++;
	}
	if (colider->GetTag() == ObjectTag::EnemyAttack)
	{
		if (m_isSpinFlag)
		{
			PlaySoundMem(m_parrySEHandle, DX_PLAYTYPE_BACK);
			auto attackSphere = dynamic_pointer_cast<EnemySphere>(colider);
			attackSphere->SetVelocity(attackSphere->GetVelocity() * -1);
			attackSphere->SetCounterFlag();
		}
		else
		{
			PlaySoundMem(m_hitSEHandle, DX_PLAYTYPE_BACK);
			colider->GetRigidbody()->SetVelocity((colider->GetRigidbody()->GetVelocity()) * -1);
			StartJoypadVibration(DX_INPUT_PAD1, 300, 600);
			auto attackSphere = dynamic_pointer_cast<EnemySphere>(colider);
			attackSphere->DeleteFlag();
			m_prevUpdate = m_playerUpdate;
			m_playerUpdate = &Player::DamegeUpdate;
			m_Hp -= 10;
			m_isOnDamageFlag = true;
			m_damageFrame = kDamageFrameMax;
			ChangeAnim(kAnimationNumHit);
		}
	}
	if (colider->GetTag() == ObjectTag::ClearObject)
	{
		m_isClearFlag = true;
	}
	if (m_Hp <= 0)
	{
		m_Hp = 0;
		m_color = 0xff0000;
	}
}

void Player::OnCollideStay(std::shared_ptr<Collidable> colider, MyEngine::ColliderBase::ColideTag tag)
{
	
}

Vec3 Player::GetCameraToPlayer() const
{
	return m_cameraToPlayer;
}

bool Player::UpdateAnim(int attachNo)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_modelHandle, attachNo);//現在の再生カウント
	now += kAnimFrameSpeed*m_speed / kFrameParSecond;//アニメーションカウントを進める


	//現在再生中のアニメーションの総カウントを取得する
	float total = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;
	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	MV1SetAttachAnimTime(m_modelHandle, attachNo, now);

	return isLoop;
}

void Player::ChangeAnim(int animIndex, int speed)
{
	m_speed = speed;
	//さらに古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevAnimNo);
	}

	//現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;

	//変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, animIndex, -1, false);

	//切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlendRate = 0.0f;

	//変更前のアニメーション100%
	DxLib::MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	DxLib::MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
}

void Player::StartUpdate()
{
	m_regeneRange += 0.01f;
	if (m_regeneRange > 2)
	{
		m_playerUpdate = &Player::NeutralUpdate;
	}
}

void Player::NeutralUpdate()
{
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	m_attackRadius = 0;
	item->radius = m_attackRadius;
	//アナログスティックを使って移動

	Vec3 move = Move();

	if (std::abs(move.Length()) >= 0.2f*kMaxSpeed)
	{
		ChangeAnim(kAnimationNumRun);
		m_playerUpdate = &Player::WalkingUpdate;
	}
	//プレイヤーの最大移動速度は0.01f/frame
	if (Pad::IsTrigger(PAD_INPUT_1))//XBoxのAボタン
	{
		ChangeAnim(kAnimationNumJump);
		m_isJumpFlag = true;
		move += m_upVec.GetNormalized() * kJumpPower;
		m_playerUpdate = &Player::JumpingUpdate;
	}
	if (Pad::IsTrigger(PAD_INPUT_B))//XBoxの
	{
		ChangeAnim(kAnimationNumSpin,5.f);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		m_attackRadius = kNetralRadius + 10;
		item->radius = m_attackRadius;
		m_isSpinFlag = true;
		m_playerUpdate = &Player::SpiningUpdate;
	}
	if (Pad::IsTrigger(PAD_INPUT_Y))
	{
		m_playerUpdate = &Player::AimingUpdate;
	}
	
	m_rigid->SetVelocity(move);
}

void Player::WalkingUpdate()
{
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	m_attackRadius = 0;
	item->radius = m_attackRadius;
	
	Vec3 ans;

	ans = Move();

	if (std::abs(ans.Length()) < 0.2f*kMaxSpeed)
	{
		ChangeAnim(kAnimationNumIdle);
		m_playerUpdate = &Player::NeutralUpdate;
	}


	if (Pad::IsTrigger(PAD_INPUT_1))//XBoxのAボタン
	{
		ChangeAnim(kAnimationNumJump);
		m_isJumpFlag = true;
		ans += m_upVec.GetNormalized() * kJumpPower;
		m_playerUpdate = &Player::JumpingUpdate;
	}
	if (Pad::IsTrigger(PAD_INPUT_B))//XBoxの
	{
		ChangeAnim(kAnimationNumSpin,5.f);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		m_attackRadius = kNetralRadius * 1.5f;
		item->radius = m_attackRadius;
		m_isSpinFlag = true;
		m_playerUpdate = &Player::SpiningUpdate;
	}
	if (Pad::IsTrigger(PAD_INPUT_Y))
	{
		m_playerUpdate = &Player::AimingUpdate;
	}
	m_rigid->SetVelocity(ans);
}

void Player::JumpingUpdate()
{
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	m_attackRadius = 0;
	item->radius = m_attackRadius;
	
	if (Pad::IsTrigger(PAD_INPUT_B))//XBoxの
	{
		ChangeAnim(kAnimationNumSpin,5.f);
		if (m_spinCount >= 1)return;
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		m_attackRadius = kNetralRadius *1.5f;
		item->radius = m_attackRadius;
		m_isSpinFlag = true;
		m_spinCount++;
		m_playerUpdate = &Player::JumpingSpinUpdate;
	}
}

void Player::AimingUpdate()
{
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	m_attackRadius = 0;
	item->radius = m_attackRadius;
	
	Vec3 move;
	move = Move();
	//プレイヤーの最大移動速度は0.01f/frame
	if (Pad::IsTrigger(PAD_INPUT_1))//XBoxのAボタン
	{
		ChangeAnim(kAnimationNumJump);
		m_isJumpFlag = true;
		move += m_upVec.GetNormalized() * kJumpPower;
		m_playerUpdate = &Player::JumpingUpdate;
	}
	if (Pad::IsTrigger(PAD_INPUT_B))//XBoxの
	{
		ChangeAnim(kAnimationNumSpin);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		m_attackRadius = kNetralRadius + 10;
		item->radius = m_attackRadius;
		m_isSpinFlag = true;
		m_playerUpdate = &Player::SpiningUpdate;
	}
	if (Pad::IsTrigger(PAD_INPUT_Y))
	{
		m_playerUpdate = &Player::NeutralUpdate;
	}
	
	m_rigid->SetVelocity(move);
	
}

void Player::SpiningUpdate()
{
	Vec3 move;

	move = Move();

	m_rigid->SetVelocity(move);

	m_spinAngle += DX_PI_F / 15;
	m_angle += DX_PI_F / 15;
	if (m_spinAngle >= DX_PI_F * 2)
	{
		ChangeAnim(kAnimationNumIdle);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		m_attackRadius = 0;
		item->radius = m_attackRadius;
		m_isSpinFlag = false;
		m_playerUpdate = &Player::NeutralUpdate;
		m_spinAngle = 0;
	}
}

void Player::JumpingSpinUpdate()
{
	//アナログスティックを使って移動

	Vec3 move;
	move = Move();

	m_rigid->SetVelocity(move);

	m_spinAngle += DX_PI_F / 15;
	m_angle += DX_PI_F / 15;
	m_spinAngle += DX_PI_F / 15;
	m_angle += DX_PI_F / 15;
	if (m_spinAngle >= DX_PI_F * 2)
	{
		ChangeAnim(kAnimationNumJump);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		m_attackRadius = 0;
		item->radius = m_attackRadius;
		m_isSpinFlag = false;
		m_playerUpdate = &Player::JumpingUpdate;
		m_spinAngle = 0;
	}
}

void Player::CommandJump()
{
	ChangeAnim(kAnimationNumJump);
	m_isJumpFlag = true;
	m_rigid->AddVelocity(m_upVec.GetNormalized() * kJumpPower);
	m_playerUpdate = &Player::JumpingUpdate;
}

void Player::BoostUpdate()
{
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	m_attackRadius = 0;
	item->radius = m_attackRadius;
	
	if (!m_isBoostFlag)
	{
		ChangeAnim(kAnimationNumIdle);
		m_playerUpdate = &Player::NeutralUpdate;
	}
}

void Player::OperationUpdate()
{
	if (!m_isOperationFlag)
	{
		SetAntiGravity(false);
		ChangeAnim(kAnimationNumIdle);
		m_playerUpdate = &Player::JumpingUpdate;
	}
}

Vec3 Player::Move()
{
	int analogX = 0, analogY = 0;
	GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);
	analogY = -analogY;

	m_inputVec.x=analogX;
	m_inputVec.z = analogY;

	

	Vec3 ans;  // 初期化はそのままに
	Vec3 modelDir;
	// アナログスティックの入力を反映
	m_sideVec = GetCameraRightVector();
	m_frontVec = Cross(m_sideVec, m_upVec).GetNormalized();

	ans = m_frontVec * static_cast<float>(analogY);
	ans += m_sideVec * static_cast<float>(analogX);
	modelDir = m_frontVec * static_cast<float>(analogY);
	modelDir -= m_sideVec * static_cast<float>(analogX);
	
	if (ans.Length() > 0)
	{
		m_moveDir = ans.GetNormalized();
		m_inputVec = modelDir.GetNormalized();
	}
	// 無効範囲の確認
	float len = ans.Length();
	if (len < kAnalogRangeMin) {
		return Vec3(0, 0, 0);  // 無効な場合はゼロベクトルを返す
	}

	if (ans.Length() > 0)
	{
		m_moveDir = ans.GetNormalized();
		m_inputVec = modelDir.GetNormalized();
	}

	ans = ans.GetNormalized() * kMaxSpeed; // 正規化し、速度を掛ける
	return ans;
}

void Player::ShotTheStar()
{
	Vec3 shotPos = m_rigid->GetPos();
	shotPos += m_upVec.GetNormalized() * 7;
	m_sphere.push_back(std::make_shared<PlayerSphere>(Priority::Low, ObjectTag::PlayerBullet, shared_from_this(), shotPos, m_shotDir,m_sideVec, 1, 0xff0000));
	MyEngine::Physics::GetInstance().Entry(m_sphere.back());
}

void Player::DamegeUpdate()
{
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	m_attackRadius = 0;
	item->radius = m_attackRadius;
	m_rigid->SetVelocity(m_rigid->GetVelocity() * 0.8f);
	if (m_rigid->GetVelocity().Length() < 7.0f)
	{
		if (m_prevUpdate != m_playerUpdate)
		{
			ChangeAnim(kAnimationNumRun);
			//ダメージアニメーションのみ
			m_playerUpdate = m_prevUpdate;
		}
		else
		{
			ChangeAnim(kAnimationNumIdle);
			m_playerUpdate = &Player::NeutralUpdate;
			m_prevUpdate 
				= m_playerUpdate;
		}
	}
}
void Player::AvoidUpdate()
{
	actionFrame++;

	if (actionFrame > kAvoidFrame)
	{
		actionFrame = 0;
		m_radius = kNetralRadius;
		ChangeAnim(kAnimationNumIdle);
		m_playerUpdate = &Player::NeutralUpdate;
	}
}

void Player::Planet1Update()
{
}

void Player::SetShotDir()
{
	int directX = 0, directY = 0;
	GetJoypadAnalogInputRight(&directX, &directY, DX_INPUT_PAD1);
	directY = -directY;

	m_shotDir = m_sideVec*static_cast<float>(directX) * 0.001f;
	m_shotDir = m_shotDir+(m_upVec*static_cast<float>(directY) * 0.0005f);
	m_shotDir = m_shotDir + m_frontVec;
	m_shotDir = m_shotDir.GetNormalized();
}

void Player::DeleteManage()
{
	auto result = remove_if(m_sphere.begin(), m_sphere.end(), [this](const auto& sphere)
		{
			bool isOut = sphere->IsDelete() == true;
	if (isOut == true)
	{
		MyEngine::Physics::GetInstance().Exit(sphere);
	}
	return isOut;
		});
	m_sphere.erase(result, m_sphere.end());
}

