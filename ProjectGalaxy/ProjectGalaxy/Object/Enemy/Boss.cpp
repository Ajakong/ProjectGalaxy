﻿#include "Boss.h"
#include"Planet.h"
#include"Player.h"
#include"Physics.h"
#include"SoundManager.h"
#include"Easing.h"
namespace
{
	constexpr int kHPFull = 60;
	constexpr float kBodyRadiusSize = 20.f;
	constexpr int kKnockBackFrameMax = 50;

	constexpr float kAnimFrameSpeed = 30.0f;//アニメーション進行速度

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	constexpr float kFrameParSecond = 60.0f;
	constexpr float kRunningSpeed = 3.0f;


	constexpr int kTackleMaxChargeFrame = 60;
	constexpr int kRunningFrameMax = 400;
	constexpr int kActionFrame = 100;

	constexpr int kTackleTime = 20;
	constexpr int kTackleSpeed = 5;

	constexpr int kTackleLength = kTackleSpeed *kTackleTime;

	const char* kDamageSEName = "Parry.mp3";
	const char* kCriticalHitSEName = "CounterHit.mp3";

}
Boss::Boss(Vec3 pos, std::shared_ptr<Player>player):Enemy(Priority::Boss,ObjectTag::Boss),
	m_jumpCount(0),
	m_actionFrame(0),
	m_tackleChargeFrame(0),
	m_isHit(false),
	m_onColStage(false),
	m_knockBackFrame(0),
	m_runningFrame(0),
	m_damageSoundHandle(SoundManager::GetInstance().GetSoundData(kDamageSEName)),
	m_criticalHandle(SoundManager::GetInstance().GetSoundData(kCriticalHitSEName))
{
	m_player = player;
	m_hp = kHPFull;
	m_rigid->SetPos(pos);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere, ColideTag::Body);
	m_collision = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back()->col);
	m_collision->radius = kBodyRadiusSize;

	m_color = 0xff00ff;
	m_bossUpdate = &Boss::NeutralUpdate;
	m_phaseUpdate = &Boss::PhaseTwoUpdate;
}

Boss::~Boss()
{
}

void Boss::Init()
{
}

void Boss::Update()
{
	//上方向ベクトルをいい感じに線形保管
	m_upVec = Slerp(m_upVec, m_nextUpVec, 1.f);
	(this->*m_bossUpdate)();
	if (m_isHit)
	{
		m_knockBackFrame++;
		m_color = 0xff0000;
	}
	if (m_knockBackFrame > 30)
	{
		m_isHit = false;
		m_color = 0xff00ff;
	}
	if (m_hp <= 0)
	{
		m_dropItem = std::make_shared<ClearObject>(m_rigid->GetPos(), true);
		Physics::GetInstance().Entry(m_dropItem);
		m_isDestroyFlag = true;
	}
	DeleteObject(m_impacts);

	m_onColStage = false;
}

void Boss::Draw()
{
	DrawSphere3D(m_rigid->GetPos().VGet(), kBodyRadiusSize,8,m_color,0x000000,true);
	
}

void Boss::PhaseOneUpdate()
{
	m_actionFrame++;
	if (m_actionFrame < kActionFrame)return;
	m_actionFrame = 0;
	switch (GetRand(1))
	{
	case(0):
		m_rigid->AddVelocity(m_upVec * 2);
		m_bossUpdate = &Boss::JumpingUpdate;
		break;
	case(1):
		m_rigid->AddVelocity(m_upVec * 4);
		m_bossUpdate = &Boss::FullpowerJumpUpdate;
		break;
	}

}

void Boss::PhaseTwoUpdate()
{
	m_actionFrame++;
	if (m_actionFrame < kActionFrame)return;
	m_actionFrame = 0;
	Vec3 ToTargetVec = (m_player->GetPos() - m_rigid->GetPos());
	if (ToTargetVec.Length() < kTackleLength)
	{
		switch (GetRand(1))
		{
		case(0):
			m_bossUpdate = &Boss::TackleUpdate;
			break;
		case(1):
			m_runningDir=ToTargetVec.GetNormalized();
			m_bossUpdate = &Boss::RunningUpdate;
			break;
		}
	}
	else
	{
		switch (GetRand(1))
		{
		case(0):
			m_rigid->AddVelocity(m_upVec * 2);
			m_bossUpdate = &Boss::JumpingUpdate;
			break;
		case(1):
			m_rigid->AddVelocity(m_upVec * 4);
			m_bossUpdate = &Boss::FullpowerJumpUpdate;
			break;
		}
	}
	

}

void Boss::PhaseThreeUpdate()
{
}

void Boss::InitUpdate()
{
}

void Boss::RestUpdate()
{

}

void Boss::NeutralUpdate()
{
	(this->*m_phaseUpdate)();
}

void Boss::AnglyUpdate()
{
}

void Boss::DestroyPlanetUpdate()
{
}

void Boss::KnockBackUpdate()
{
	m_color = 0xffff00;
	m_state = State::Attack;
	m_knockBackFrame++;
	if (m_knockBackFrame > kKnockBackFrameMax)
	{
		m_color = 0xff00ff;
		m_knockBackFrame = 0;
		m_bossUpdate = &Boss::NeutralUpdate;
	}
}

void Boss::JumpingUpdate()
{
	m_color = 0xffff00;
	m_state = State::Attack;
	if (m_onColStage)
	{
		if (m_jumpCount > 2)
		{
			m_jumpCount = 0;
			switch (GetRand(1))
			{
			case(0):
				m_bossUpdate = &Boss::NeutralUpdate;
				break;
			case(1):
				m_rigid->AddVelocity(m_upVec * 4);
				m_bossUpdate = &Boss::FullpowerJumpUpdate;
				break;
			}

		}
		else
		{
			m_jumpCount++;
			m_impacts.push_back(std::make_shared<StampImpact>(m_rigid->GetPos() + m_upVec * -kBodyRadiusSize, m_nowPlanet->GetScale(), m_upVec * -1, ObjectTag::Electronic));
			MyEngine::Physics::GetInstance().Entry(m_impacts.back());
			m_rigid->AddVelocity(m_upVec * 2);
		}
		
	}
	
}

void Boss::FullpowerJumpUpdate()
{
	m_color = 0xffff00;
	m_state = State::Attack;
	if (m_onColStage)
	{
		m_impacts.push_back(std::make_shared<StampImpact>(m_rigid->GetPos() + m_upVec * -kBodyRadiusSize, m_nowPlanet->GetScale(), m_upVec * -1, ObjectTag::Electronic,2.f));
		MyEngine::Physics::GetInstance().Entry(m_impacts.back());

		//HPが少ないほど隙がなくなる
		m_actionFrame = -m_hp;
		m_bossUpdate = &Boss::LandingUpdate;

	}
}

void Boss::TackleUpdate()
{
	m_color = 0xffff00;
	m_state = State::Tackle;

	Vec3 targetDir = m_player->GetPos() - m_rigid->GetPos();
	targetDir.Normalize();

	m_tackleChargeFrame++;
	m_rigid->SetVelocity(m_upVec * 0.5f);
	if (m_tackleChargeFrame > kTackleMaxChargeFrame)
	{
		
		m_rigid->SetVelocity(targetDir * kTackleSpeed);
		if (m_tackleChargeFrame - kTackleMaxChargeFrame > kTackleTime)
		{
			m_tackleChargeFrame = 0;
			//HPが少ないほど隙がなくなる
			m_actionFrame = -m_hp;
			m_bossUpdate = &Boss::LandingUpdate;
		}
	}
}

void Boss::RunningUpdate()
{
	m_color = 0xffff00;
	m_state = State::Running;

	m_sideVec = Cross(m_upVec, m_runningDir);
	m_rigid->SetVelocity(m_sideVec*kRunningSpeed);

	m_runningFrame++;
	if (m_runningFrame > kRunningFrameMax / 2)
	{
		m_rigid->SetVelocity(m_rigid->GetVelocity() * -1);
	}
	if (m_runningFrame > kRunningFrameMax)
	{
		m_runningFrame = 0;
		//HPが少ないほど隙がなくなる
		m_actionFrame = -m_hp;
		m_bossUpdate = &Boss::LandingUpdate;
	}

}

void Boss::LandingUpdate()
{
	m_color = 0x00ff00;
	m_actionFrame++;
	if (m_actionFrame > 0)
	{
		m_color = 0xff00ff;
		m_bossUpdate = &Boss::NeutralUpdate;
	}
}


bool Boss::UpdateAnim(int attachNo)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_modelHandle, attachNo);//現在の再生カウント
	now += kAnimFrameSpeed * m_animationSpeed / kFrameParSecond;//アニメーションカウントを進める


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

void Boss::ChangeAnim(int animIndex, int speed)
{
	m_animationSpeed = speed;
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

void Boss::OnCollideEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag)
{
	if (colider->GetTag() == ObjectTag::Stage)
	{
		m_onColStage = true;
	}
	if (m_bossUpdate == &Boss::LandingUpdate)
	{
		if (colider->GetTag() == ObjectTag::PlayerBullet)
		{
			m_isHit = true;
			PlaySoundMem(m_damageSoundHandle, DX_PLAYTYPE_BACK);
			m_hp -= 5;
		}
	}
	if (colider->GetTag() == ObjectTag::Player)
	{
		auto state = GetState();
		if ((state == State::Running || state == State::Tackle) && colider->GetState() == State::Spin)
		{
			PlaySoundMem(m_criticalHandle, DX_PLAYTYPE_BACK);
			Vec3 dir = colider->GetRigidbody()->GetPos();
			dir.Normalize();
			m_rigid->SetVelocity((dir + m_upVec) * 2);
			//HPが少ないほど隙がなくなる
			m_actionFrame = -m_hp-200;
			m_bossUpdate = &Boss::LandingUpdate;
		}
	}
}

void Boss::OnTriggerEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag)
{
	if (colider->GetTag() == ObjectTag::Stage)
	{
		m_nowPlanet = std::dynamic_pointer_cast<Planet>(colider);
	}
	if (m_bossUpdate == &Boss::LandingUpdate)
	{
		if (colider->GetTag() == ObjectTag::PlayerImpact)
		{
			m_rigid->AddVelocity(m_upVec * 4);
			m_hp -= 20;
			PlaySoundMem(m_criticalHandle, DX_PLAYTYPE_BACK);
		}
		if (colider->GetTag() == ObjectTag::PlayerBullet)
		{
			m_hp -= 1;
		}
	}
}

template <typename T>
void Boss::DeleteObject(std::vector<std::shared_ptr<T>>& objects)
{
	auto result = remove_if(objects.begin(), objects.end(), [this](const auto& object)
		{
			if (object->GetDeleteFlag())
			{
				MyEngine::Physics::GetInstance().Exit(object);
			}
			return object->GetDeleteFlag(); // IsDestroy() が true の場合削除
		});
	objects.erase(result, objects.end());
}


