#include "PlayerStickSphere.h"
#include"ColliderSphere.h"
#include"Player.h"
#include"Item.h"

#include"SoundManager.h"
#include"ScreenManager.h"
#include"EffectManager.h"

#include"Game.h"
namespace
{
	/// <summary>
	/// 球の当たり判定半径
	/// </summary>
	constexpr float kSphereRadius = 0.3f;
	/// <summary>
	/// 球の生成間隔
	/// </summary>
	constexpr int kSphereCreateFrame = 50;

	constexpr int kLifeTimeMax = 100;
	const char* kName = "Sphere";
	const char* kLineEffectName = "Line.efk";
	const char* kSphereEffectName = "StickSphere.efk";
	const char* kBoostEffectName = "BoostEffect.efk";

	const char* kOperationSEName = "Fastener.mp3";

	constexpr float kTensionSpeed = 3.f;

	const char* kGaussFilterScreenName = "GaussOfSeekerLine";
	const char* kHighBrightScreenName = "HighBrightSeekerLine";
}

PlayerStickSphere::PlayerStickSphere(MyEngine::Collidable::Priority priority, ObjectTag tag, std::shared_ptr<MyEngine::Collidable> player, Vec3 pos, Vec3 velocity, Vec3 sideVec, int moveNum, int color) : PlayerSphere(priority, tag, player, pos, velocity, sideVec, moveNum, color),
m_sideVec(sideVec),
m_lifeTime(0),
m_pushCount(0),
m_lineEffectIndex(0),
m_sphereEffectIndex(0),
m_boostEffectIndex(0),
m_operationHandle(SoundManager::GetInstance().GetSoundData(kOperationSEName))
{
	
	m_player.lock() = std::dynamic_pointer_cast<Player>(player);
	/*m_rigid->SetVelocity(VGet(m_velocity.x * 2, m_velocity.y * 2, m_velocity.z * 2));
	m_rigid->SetPos(pos);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere, ColideTag::Body);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back()->col);
	item->radius = m_radius;
	m_color = color;*/
	//moveNumによって挙動が変わるのかもしれない(実装するかわからん)
	//if (moveNum == 0)
	{
		m_moveUpdate = &PlayerStickSphere::StraightUpdate;
	}

	SetAntiGravity(true);

	m_emitPoint = m_player.lock()->GetLeftHandPos();
}


PlayerStickSphere::~PlayerStickSphere()
{
	EffectManager::GetInstance().StopEffect(kLineEffectName, m_lineEffectIndex);
	EffectManager::GetInstance().StopEffect(kSphereEffectName, m_sphereEffectIndex);
	EffectManager::GetInstance().StopEffect(kBoostEffectName, m_boostEffectIndex);
}

void PlayerStickSphere::Init()
{
	m_lineEffectIndex = EffectManager::GetInstance().PlayEffect(kLineEffectName, true, 0, shared_from_this());
	m_sphereEffectIndex = EffectManager::GetInstance().PlayEffect(kSphereEffectName, true, 0, shared_from_this());
	m_boostEffectIndex = EffectManager::GetInstance().PlayEffect(kBoostEffectName, true, 0, shared_from_this());
}

void PlayerStickSphere::Update()
{
	m_startPos = m_player.lock()->GetLeftHandPos();
	(this->*m_moveUpdate)();
}

void PlayerStickSphere::Draw()
{
	//弾の描画
	EffectManager::GetInstance().SetPositionEffect(kSphereEffectName, m_sphereEffectIndex, m_rigid->GetPos(), MScale(MGetIdent(),0.3f));
	
	//m_isStickがtrueの時に赤くなる
	DrawLine3D(m_startPos.VGet(), m_rigid->GetPos().VGet(), 0xffffff - (0x00ffff * m_isStick));

	auto lineLength = (m_startPos - m_emitPoint).Length();
	for (int i = 0; i < lineLength; i+=5)
	{
		DrawSphere3D((m_emitPoint + (m_startPos - m_emitPoint).GetNormalized() * i).VGet(), 0.1f, 10, 0x00ffff, 0x00ffff, true);
	}
	//ラインの描画
	EffectManager::GetInstance().SetPositionEffect(kLineEffectName, m_lineEffectIndex, m_player.lock()->GetPos(), MGetIdent());
	
	auto mat = MGetRotVec2(m_startPos.GetNormalized().VGet(), m_rigid->GetPos().GetNormalized().VGet());
	//プレイヤーの移動軌跡の描画
	EffectManager::GetInstance().SetPositionEffect(kBoostEffectName, m_boostEffectIndex, m_player.lock()->GetPos(), mat);
}

void PlayerStickSphere::Effect()
{
	if (m_isStick)
	{
		if (m_isMoving)
		{
			m_moveUpdate = &PlayerStickSphere::ComeBackWithObjectUpdate;
		}
		else if ((m_player.lock()->GetPos() - m_rigid->GetPos()).Length() < 20)
		{
			m_moveUpdate = &PlayerStickSphere::ComeBackUpdate;
		}
		else
		{
			
			//Aが入力された回数(押された分速度が上がる)
			m_pushCount++;
			
			//プレイヤーを操作されている状態に移行する
			m_player.lock()->SetIsOperation(true);
			
			//Playerを引っ張る方向
			Vec3 tensionDir = (m_rigid->GetPos() - m_player.lock()->GetPos()).GetNormalized();
			m_player.lock()->SetVelocity(tensionDir * kTensionSpeed * static_cast<float>(m_pushCount));
		}
		
	}
	else m_moveUpdate = &PlayerStickSphere::ComeBackUpdate;
}

void PlayerStickSphere::OnCollideEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag)
{
	m_isStick = true;
	auto priority = colider->GetPriority();
	m_isMoving = priority == Priority::Lowest;
	m_moveUpdate = &PlayerStickSphere::StickUpdate;
	m_contactedCollidable = colider;
	m_collidableContactPosition = colider->GetRigidbody()->GetPos() - m_rigid->GetPos();
}

void PlayerStickSphere::OnTriggerEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag)
{
	auto ptr = std::dynamic_pointer_cast<Item>(colider);
	if (ptr != nullptr)
	{
		m_isStick = true;
		auto priority = colider->GetPriority();
		m_isMoving = priority == Priority::Lowest;
		m_moveUpdate = &PlayerStickSphere::StickUpdate;
		m_contactedCollidable = colider;
		m_collidableContactPosition = colider->GetRigidbody()->GetPos() - m_rigid->GetPos();
	}
}

void PlayerStickSphere::StraightUpdate()
{
	m_rigid->SetVelocity(m_velocity*4);
}

void PlayerStickSphere::StickUpdate()
{
	SetAntiGravity(true);
	m_rigid->SetVelocity(Vec3::Zero());
	
	//m_rigid->SetPos(m_contactedCollidable->GetRigidbody()->GetPos() + m_collidableContactPosition);
	if ((m_rigid->GetPos() - m_startPos).Length() <= 5.0f)
	{
		m_player.lock()->SetIsOperation(false);
		m_moveUpdate = &PlayerStickSphere::ComeBackUpdate;
	}
}

void PlayerStickSphere::ComeBackUpdate()
{
	
	float speed = (m_startPos - m_rigid->GetPos()).Length()/10;
	m_rigid->SetVelocity((m_startPos - m_rigid->GetPos()).GetNormalized() * speed);
	if ((m_rigid->GetPos() - m_startPos).Length() <= 2.f)
	{

		StopSoundMem(m_operationHandle);
		m_isDestroy = true;
	}
}

void PlayerStickSphere::ComeBackWithObjectUpdate()
{
	m_rigid->SetVelocity((m_startPos - m_rigid->GetPos()).GetNormalized() * 3);
	m_contactedCollidable->GetRigidbody()->SetPos(m_rigid->GetPos()+m_collidableContactPosition);
	if ((m_rigid->GetPos() - m_startPos).Length() <= 1.2f)
	{
		m_isDestroy = true;
	}
}
