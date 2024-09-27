#include "Player.h"
#include"Pad.h"
#include"MyLib/Physics/ColliderSphere.h"
#include"Camera.h"
#include"SoundManager.h"
#include"Enemy/Gorori.h"
#include"Enemy/EnemySphere.h"
#include"Enemy/KillerTheSeeker.h"
#include"ModelManager.h"
#include"Quaternion.h"

/// <summary>
/// やること:足の当たり判定を生成・踏みつけに使う
/// スピン専用の当たり判定を生成・体より半径が大きい当たり判定にし、スピン中にだけ出現
/// </summary>

namespace
{
	constexpr int kNetralRadius = 20;//通常時の当たり半径

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
	constexpr float kMaxSpeed = 5.0f;//プレイヤーの最大速度
	constexpr float kAnalogRangeMin = 0.1f;//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;//アナログスティックから入力されるベクトルの最大値

	constexpr float kFrameParSecond = 60.0f;

	constexpr int kAvoidFrame = 60;

	constexpr int kJumpPower = 50;

	constexpr int kSearchRemainTimeMax = 28;
	constexpr int kChargeRemainCoolTime = 10;

	const char* kGororiHitSEName = "GororiHitSE.mp3";
	const char* kGetItemSEName = "GetItemSE.mp3";
	const char* kOnParrySEName = "Parry.mp3";

	const char* kGetSearchSEName = "Search.mp3";
	const char* name = "Player";
	const char* kFileName = "";
	constexpr int kAnimationNumTpose = 0;
	constexpr int kAnimationNumHit = 1;
	constexpr int kAnimationNumJump = 2;
	constexpr int kAnimationNumRun = 3;
	constexpr int kAnimationNumSpin = 4;
	constexpr int kAnimationNumIdle = 5;
}

float GetAngle(Vec3 a, Vec3 b)
{
	return acos(Dot(a.GetNormalized(), b.GetNormalized())) * 180 / DX_PI_F;
}

Player::Player(int modelhandle) : Collidable(Priority::High, ObjectTag::Player),
m_modelHandle(MV1DuplicateModel(ModelManager::GetInstance().GetModelData(kFileName))),
m_anim_move(),
m_radius(kNetralRadius),
m_Hp(50),
m_damageFrame(0),
m_playerUpdate(&Player::StartUpdate),
m_prevUpdate(&Player::StartUpdate),
m_cameraUpdate(&Player::Planet1Update),
m_regeneRange(0),
m_angle(0),
m_spinAngle(0),
m_animBlendRate(0),
m_currentAnimNo(-1),
m_prevAnimNo(0),
m_isJumpFlag(false),
m_isSpinFlag(false),
m_nowPlanetPos(Vec3(Vec3(0, -500, 0))),
m_searchRemainTime(0),
m_chargeRemainTime(0),
m_hitSEHandle(SoundManager::GetInstance().GetSoundData(kGororiHitSEName)),
m_color(0x00ffff),
m_getItemHandle(SoundManager::GetInstance().GetSoundData(kGetItemSEName)),
m_searchSEHandle(SoundManager::GetInstance().GetSoundData(kGetSearchSEName)),
m_attackRadius(0),
m_parrySEHandle(SoundManager::GetInstance().GetSoundData(kOnParrySEName)),
m_damageFrameSpeed(1),
m_postUpVec(Vec3::Up())
{
	{
		m_rigid->SetPos(Vec3(0, 0, 0));
		AddCollider(MyEngine::ColliderBase::Kind::Sphere);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		item->radius = m_radius;
	}

	{
		AddCollider(MyEngine::ColliderBase::Kind::Sphere);
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		item->radius = m_attackRadius;
	}

	MV1SetScale(m_modelHandle, VGet(0.05f, 0.05f, 0.05f));
	ChangeAnim(kAnimationNumIdle);	
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
	(this->*m_playerUpdate)();
	m_chargeRemainTime++;
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
	//(this->*m_cameraUpdate)();
	//m_camera->SetUpVec(GetCameraUpVector());
	//m_camera->Update(m_rigid->GetPos()+Vec3(GetCameraUpVector())*300);

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
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
	m_animBlendRate += 0.05f;
	if (m_animBlendRate > 1.0f)
	{
		m_animBlendRate = 1.0f;
	}

	//SetLightPositionHandle(m_pointLightHandle, m_rigid->GetPos().VGet());
}

void Player::SetMatrix()
{
	Set3DSoundListenerPosAndFrontPosAndUpVec(m_rigid->GetPos().VGet(), Vec3(m_rigid->GetPos() + GetCameraFrontVector()).VGet(), m_upVec.VGet());

	MATRIX mat;
	mat = MGetRotY(acos(Dot(Vec3::Front(), m_moveDir* -1 )));
	//mat = MGetRotX(DX_PI_F/2/*atan2(Vec3::Up().y, m_rigid->GetPos().z)*/);
	//mat = MMult(mat, MGetRotZ(0/*atan2(Vec3::Up().y, m_rigid->GetPos().x)*/));
	Vec3 axis = Cross(Vec3::Up(), m_upVec);

	mat=MMult(mat, MGetRotVec2(Vec3::Up().VGet(), m_upVec.VGet()));

	/*mat =;*/

	//mat = MMult(mat, MGetRotVec2(Vec3::Front().VGet(), (m_moveDir).VGet()));
//	////カメラのいる角度から
//	////コントローラーによる移動方向を決定する
	//	Quaternion myQ;
//	myQ=myQ.CreateRotationQuaternion(acos(Dot(Vec3::Up(), m_upVec)), axis);
//	m_postUpVec = m_upVec;
//	MATRIX mat;
//	MATRIX matNorm = MGetRotVec2(Vec3::Up().VGet(), m_upVec.VGet());
//	MATRIX rot=MGetRotElem(matNorm);
//	MATRIX matFront = MGetRotVec2(Vec3::Front().VGet(), m_frontVec.VGet());
//	MGetRotElem(matNorm);
//	MATRIX matAns = MMult(matNorm,matFront);
//
//
//	
//
//#if _DEBUG
//
//	
//#endif
//	/*MATRIX scale = MGetScale(VGet(0.05f,0.05f,0.05f));
//	MATRIX rot= myQ.ToMat();
//	MATRIX trans = MGetTranslate(m_rigid->GetPos().VGet());
//	mat = scale;
//	mat = MMult(scale, rot);
//	mat = MMult(mat, trans);*/
	MV1SetRotationMatrix(m_modelHandle, mat);
	MV1SetPosition(m_modelHandle, m_rigid->GetPos().VGet());

	//MV1SetMatrix(m_modelHandle, matNorm);
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


	//printfDx("%d", HitCount);
#endif
}

void Player::SetCameraToPlayer(Vec3 cameraToPlayer)
{
	m_cameraToPlayer = cameraToPlayer;
}

void Player::SetCameraAngle(float cameraAngle)
{
	m_cameraAngle = cameraAngle;
}

void Player::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Stage)
	{
		m_spinCount = 0;
		m_nowPlanetPos = colider->GetRigidbody()->GetPos();
		m_playerUpdate = &Player::NeutralUpdate;
		m_isJumpFlag = false;
		m_isBoostFlag = false;
		ChangeAnim(kAnimationNumIdle);
	}
	if (colider->GetTag() == ObjectTag::Takobo)
	{
		if (m_isSpinFlag)
		{
			PlaySoundMem(m_parrySEHandle, DX_PLAYTYPE_BACK);
			colider->GetRigidbody()->SetVelocity(Vec3(m_rigid->GetPos() - colider->GetRigidbody()->GetPos()).GetNormalized() * -30);
		}
		else
		{
			PlaySoundMem(m_hitSEHandle, DX_PLAYTYPE_BACK);
			StartJoypadVibration(DX_INPUT_PAD1, 600, 600);
			m_Hp -= 10;
			m_prevUpdate = m_playerUpdate;
			m_playerUpdate = &Player::DamegeUpdate;
			m_rigid->AddVelocity(Vec3(m_rigid->GetPos() - colider->GetRigidbody()->GetPos()).GetNormalized() * 30);
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
			m_rigid->AddVelocity(Vec3(m_rigid->GetPos() - colider->GetRigidbody()->GetPos()).GetNormalized() * 40);
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
			m_rigid->AddVelocity(Vec3(m_rigid->GetPos() - colider->GetRigidbody()->GetPos()).GetNormalized() * 40);
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

void Player::OnCollideStay(std::shared_ptr<Collidable> colider)
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
	now += kAnimFrameSpeed / kFrameParSecond;//アニメーションカウントを進める


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

void Player::ChangeAnim(int animIndex)
{
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
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);

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

	int analogX = 0, analogY = 0;

	GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);
	analogY = -analogY;

	
	//アナログスティックの入力10%~80%を使用する
	//ベクトルの長さが最大1000になる
	//ベクトルの長さを取得
	Vec3 move;

	float len = move.Length();
	//ベクトルの長さを0.0~1.0の割合に変換する
	float rate = len / kAnalogInputMax;
	Vec3 front = GetCameraFrontVector();
	Vec3 right = GetCameraRightVector();
	move = m_frontVec * static_cast<float>(analogY);//入力が大きいほど利教が大きい,0の時は0
	move += m_sideVec * static_cast<float>(analogX);


	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin / (kAnalogRangeMax - kAnalogRangeMin));
	rate = std::min(rate, 1.0f);
	rate = std::max(rate, 0.0f);
	if (std::abs(move.Length()) >= 0.2f)
	{
		ChangeAnim(kAnimationNumRun);
		m_playerUpdate = &Player::WalkingUpdate;
	}
	//速度が決定できるので移動ベクトルに反映
	move = move.GetNormalized();
	float speed = kMaxSpeed;

	//m_angle = fmodf(m_cameraAngle, 360);//mod:余り　
	//MATRIX rotate = MGetRotY((m_angle)-DX_PI_F / 2);//本来はカメラを行列で制御し、その行列でY軸回転
	m_moveDir = move;
	move = move * speed;
	//プレイヤーの最大移動速度は0.01f/frame
	if (Pad::IsTrigger(PAD_INPUT_1))//XBoxのAボタン
	{
		ChangeAnim(kAnimationNumJump);
		m_isJumpFlag = true;
		move += m_upVec.GetNormalized() * 10;
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
	
	m_rigid->SetVelocity(move);
}

void Player::WalkingUpdate()
{
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	m_attackRadius = 0;
	item->radius = m_attackRadius;
	//アナログスティックを使って移動

	int analogX = 0, analogY = 0;

	GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);
	analogY = -analogY;
	
	//アナログスティックの入力10%~80%を使用する
	//ベクトルの長さが最大1000になる
	//ベクトルの長さを取得
	Vec3 move;

	float len = move.Length();
	//ベクトルの長さを0.0~1.0の割合に変換する
	float rate = len / kAnalogInputMax;
	Vec3 front = GetCameraFrontVector();
	Vec3 right = GetCameraRightVector();
	move = m_frontVec * static_cast<float>(analogY);//入力が大きいほど利教が大きい,0の時は0
	move += m_sideVec * static_cast<float>(analogX);


	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin / (kAnalogRangeMax - kAnalogRangeMin));
	rate = std::min(rate, 1.0f);
	rate = std::max(rate, 0.0f);
	if (std::abs(move.Length()) < 0.2f)
	{
		ChangeAnim(kAnimationNumIdle);
		m_playerUpdate = &Player::NeutralUpdate;
	}
	//速度が決定できるので移動ベクトルに反映
	move = move.GetNormalized();
	float speed = kMaxSpeed;

	//m_angle = fmodf(m_cameraAngle, 360);//mod:余り　
	//MATRIX rotate = MGetRotY((m_angle)-DX_PI_F / 2);//本来はカメラを行列で制御し、その行列でY軸回転
	m_moveDir = move;
	move = move * speed;
	if (Pad::IsTrigger(PAD_INPUT_1))//XBoxのAボタン
	{
		ChangeAnim(kAnimationNumJump);
		m_isJumpFlag = true;
		move += m_upVec.GetNormalized() * 10;
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

	m_rigid->SetVelocity(move);
}

void Player::JumpingUpdate()
{
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	m_attackRadius = 0;
	item->radius = m_attackRadius;
	m_rigid->SetVelocity(m_rigid->GetPrevVelocity());

	if (Pad::IsTrigger(PAD_INPUT_B))//XBoxの
	{
		ChangeAnim(kAnimationNumSpin);
		if (m_spinCount >= 1)return;
		auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
		m_attackRadius = kNetralRadius + 10;
		item->radius = m_attackRadius;
		m_isSpinFlag = true;
		m_spinCount++;
		m_playerUpdate = &Player::JumpingSpinUpdate;
	}
}

void Player::SpiningUpdate()
{
	//アナログスティックを使って移動

	int analogX = 0, analogY = 0;

	GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);
	analogY = -analogY;
	//アナログスティックの入力10%~80%を使用する
	//ベクトルの長さが最大1000になる
	//ベクトルの長さを取得
	Vec3 move;

	float len = move.Length();
	//ベクトルの長さを0.0~1.0の割合に変換する
	float rate = len / kAnalogInputMax;
	Vec3 front = GetCameraFrontVector();
	Vec3 right = GetCameraRightVector();
	move = m_frontVec * static_cast<float>(analogY);//入力が大きいほど利教が大きい,0の時は0
	move += m_sideVec * static_cast<float>(analogX);

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin / (kAnalogRangeMax - kAnalogRangeMin));
	rate = std::min(rate, 1.0f);
	rate = std::max(rate, 0.0f);

	//速度が決定できるので移動ベクトルに反映
	move = move.GetNormalized();
	float speed = kMaxSpeed;

	m_moveDir = move;
	move = move * speed;

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

	int analogX = 0, analogY = 0;

	GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);
	analogY = -analogY;
	//アナログスティックの入力10%~80%を使用する
	//ベクトルの長さが最大1000になる
	//ベクトルの長さを取得
	Vec3 move;

	float len = move.Length();
	//ベクトルの長さを0.0~1.0の割合に変換する
	float rate = len / kAnalogInputMax;
	Vec3 front = GetCameraFrontVector();
	Vec3 right = GetCameraRightVector();
	move = m_frontVec * static_cast<float>(analogY);//入力が大きいほど利教が大きい,0の時は0
	move += m_sideVec * static_cast<float>(analogX);

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin / (kAnalogRangeMax - kAnalogRangeMin));
	rate = std::min(rate, 1.0f);
	rate = std::max(rate, 0.0f);

	//速度が決定できるので移動ベクトルに反映
	move = move.GetNormalized();
	float speed = kMaxSpeed;

	m_moveDir = move;
	move = move * speed;

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

void Player::BoostUpdate()
{
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	m_attackRadius = 0;
	item->radius = m_attackRadius;
	m_rigid->SetVelocity(m_rigid->GetPrevVelocity());
	if (!m_isBoostFlag)
	{
		ChangeAnim(kAnimationNumIdle);
		m_playerUpdate = &Player::NeutralUpdate;
	}
}

void Player::DamegeUpdate()
{
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	m_attackRadius = 0;
	item->radius = m_attackRadius;
	m_rigid->SetVelocity(m_rigid->GetPrevVelocity() * 0.8f);
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
			m_prevUpdate = m_playerUpdate;
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
