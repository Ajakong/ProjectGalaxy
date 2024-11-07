#include "Gorori.h"
#include"ColliderSphere.h"
#include"Physics.h"
#include"SoundManager.h"

namespace
{
	constexpr float kCollisionRadius = 50.f;

	/// <summary>
		/// 最大HP
		/// </summary>
	constexpr int kHp = 20;

	constexpr int kStartPosX = 200;
	constexpr int kStartPosY = 50;
	constexpr int kStartPosZ = 0;

	/// <summary>
	/// 足元からモデルの中心までの距離
	/// </summary>
	constexpr int kFootToCenter = 30;

	/// <summary>
	/// 攻撃クールタイム中の最低移動速度
	/// </summary>
	constexpr int kIdleSpeed = 20;
	/// <summary>
/// 球の生成間隔
/// </summary>
	constexpr int kSphereCreateFrame = 50;
	/// <summary>
	/// 再攻撃までのクールタイム
	/// </summary>
	constexpr int kAttackCoolDownTime = 300;

	/// <summary>
	/// ステージモデルの縦横サイズ/2
	/// </summary>
	constexpr int kStageSizeHalf = 200;

	const char* name = "gorori";
}

/*プロトタイプ宣言*/
Vec3 ToVec(Vec3 a, Vec3 b);
Vec3 norm(Vec3 a);
float lerp(float start, float end, float t);

Gorori::Gorori(Vec3 pos) :Enemy(-1, Priority::Static, ObjectTag::Gorori),
m_Hp(kHp),
m_attackCoolDownCount(0),
m_centerToEnemyAngle(0),
m_attackCount(0),
m_color(0xaaaa11)
{
	m_enemyUpdate = &Gorori::IdleUpdate;
	m_rigid->SetPos(pos);
	AddCollider(MyEngine::ColliderBase::Kind::Sphere, MyEngine::ColliderBase::ColideTag::Body);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kCollisionRadius;
	m_moveShaftPos = m_rigid->GetPos();
	AddThroughTag(ObjectTag::Gorori);
	AddThroughTag(ObjectTag::Takobo);
	AddThroughTag(ObjectTag::WarpGate);
	AddThroughTag(ObjectTag::EnemyAttack);
}

Gorori::~Gorori()
{

}

void Gorori::Init()
{
}

void Gorori::Update()
{
	(this->*m_enemyUpdate)();
}

void Gorori::SetMatrix()
{
	MATRIX moving = MGetTranslate(m_rigid->GetPos().VGet());

	MV1SetMatrix(m_modelHandle, moving);
}

void Gorori::Draw()
{
	DrawSphere3D(m_rigid->GetPos().VGet(), kCollisionRadius, 10,m_color, 0xff0000, true);
	MV1DrawModel(m_modelHandle);
}

void Gorori::OnCollideEnter(std::shared_ptr<Collidable> colider, MyEngine::ColliderBase::ColideTag tag)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		m_Hp -= 20;
	}
}

Vec3 Gorori::GetMyPos()
{
	return  VGet(m_rigid->GetPos().x, m_rigid->GetPos().y + kFootToCenter, m_rigid->GetPos().z);;
}

void Gorori::SetTarget(std::shared_ptr<Collidable> target)
{
	m_target = target;
}

void Gorori::IdleUpdate()
{
	m_vec.x = 1;
	if (abs(m_rigid->GetPos().x - m_moveShaftPos.x) > 5)
	{
		m_vec.x *= -1;
	}

	m_rigid->SetVelocity(VGet(m_vec.x, 0, 0));

	m_attackCoolDownCount++;

	if ((m_rigid->GetPos() - m_target->GetRigidbody()->GetPos()).Length() > 100)
	{
		if (m_attackCoolDownCount > kAttackCoolDownTime)
		{
			int attackState = GetRand(1);
			switch (attackState)
			{
			case 0:
				m_attackCoolDownCount = 0;
				m_attackDir = GetAttackDir().GetNormalized();//オブジェクトに向かうベクトルを正規化したもの
				m_enemyUpdate = &Gorori::AttackUpdate;
				m_color = 0xff0000;
				break;
			default:
				m_attackCoolDownCount =0;
				break;
			}
		}
	}
}

void Gorori::AttackUpdate()
{
	m_rigid->SetVelocity(m_attackDir * 8);
	m_attackCount++;
	if (m_attackCount > 1000)
	{
		m_attackCount = 0;
		m_color = 0xaaaa11;
		m_enemyUpdate = &Gorori::IdleUpdate;
	}
}

Vec3 Gorori::GetAttackDir() const
{
	Vec3 vec = ToVec(m_rigid->GetPos(), m_target->GetRigidbody()->GetPos()).GetNormalized();
	
	return vec;
}
