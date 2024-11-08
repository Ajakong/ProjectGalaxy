#include "PlayerSphere.h"
#include "ColliderSphere.h"
#include "PlayerSphere.h"
#include"Player.h"
namespace
{
	/// <summary>
	/// 球の当たり判定半径
	/// </summary>
	constexpr float kSphereRadius = 1.0f;
	/// <summary>
	/// 球の生成間隔
	/// </summary>
	constexpr int kSphereCreateFrame = 50;
	
	constexpr int kLifeTimeMax = 100;
	const char* name = "Sphere";

}
PlayerSphere::PlayerSphere(MyEngine::Collidable::Priority priority, ObjectTag tag, std::shared_ptr<MyEngine::Collidable>player, Vec3 pos, Vec3 velocity,Vec3 sideVec ,int moveNum, int color) : SphereBase(priority, tag, pos, velocity, color, kSphereRadius),
m_player(std::dynamic_pointer_cast<Player>(player)),
m_sideVec(sideVec),
m_lifeTime(0)
{
	m_rigid->SetPos(pos);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere, MyEngine::ColliderBase::ColideTag::Body);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = m_radius;
	m_color = color;
	SetAntiGravity(true);
	//moveNumによって挙動が変わるのかもしれない(実装するかわからん)
	//if (moveNum == 0)
	{
		m_moveUpdate = &PlayerSphere::StraightUpdate;
	}
}

PlayerSphere::~PlayerSphere()
{
}

void PlayerSphere::Init()
{
}

void PlayerSphere::Update()
{
	(this->*m_moveUpdate)();
}

void PlayerSphere::Draw()
{
	DrawSphere3D(m_rigid->GetPos().VGet(), kSphereRadius, 10, 0xffff00, m_color, false);
}

void PlayerSphere::Hit()
{
	
}

void PlayerSphere::OnCollideEnter(std::shared_ptr<Collidable> colider, MyEngine::ColliderBase::ColideTag tag)
{
	m_isDeleteFlag = true;
}

void PlayerSphere::StraightUpdate()
{
	//m_velocity = Cross(m_upVec, m_sideVec);//地表に沿う
	m_rigid->SetVelocity(VGet(m_velocity.x*2, m_velocity.y*2, m_velocity.z*2));
	m_lifeTime++;
	if (m_lifeTime > kLifeTimeMax)
	{
		m_isDeleteFlag = true;
	}
}