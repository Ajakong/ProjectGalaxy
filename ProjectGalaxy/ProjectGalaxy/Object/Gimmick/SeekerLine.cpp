#include "SeekerLine.h"
#include"Player.h"
#include"Easing.h"
#include"ColliderSphere.h"

namespace
{
	constexpr int kSpeedMax = 20;
	constexpr int kRadius = 50;
}

SeekerLine::SeekerLine(std::vector<Vec3> points, int color) : Collidable(Priority::Static, ObjectTag::SeekerLine),
m_hitPointNum(0)
{
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kRadius;
	for (auto& point : points)
	{
		m_points.push_back(point);
	}
}

SeekerLine::~SeekerLine()
{
}

void SeekerLine::Init()
{
}

void SeekerLine::Update()
{
	if (m_player == nullptr)return;
	if ((m_player->GetPos() - m_points[m_hitPointNum + 1]).Length() <= 20)
	{
		m_hitPointNum++;
		if (m_points[m_hitPointNum] == m_points.back())
		{
			//プレイヤーをジャンプさせる
			m_player->CommandJump();
			m_hitPointNum = 0;
		}
	}
	float lenge = (m_rigid->GetPos() - m_player->GetPos()).Length();
	float ratio = (lenge / m_playerStartPos.Length());
	m_ratio += 0.002f;
	if (m_ratio > 1)
	{
		m_ratio = 1;
	}
	m_player->SetVelocity(EaseInOut(m_playerStartPos, m_points[m_hitPointNum], m_ratio, 2) - m_player->GetPos());
}

void SeekerLine::Draw()
{
	for (int i = 0; i+1 < m_points.size(); i++)
	{
		DrawLine3D((m_points.begin() + i)->VGet(), (m_points.begin() + i + 1)->VGet(), m_color);
		if ((m_points.begin() + i + 1) == m_points.end())return;
	}
	DrawSphere3D(m_points[0].VGet(), kRadius, 8, 0xffff00, 0xffffff, false);
}

void SeekerLine::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		m_player = std::dynamic_pointer_cast<Player>(colider);
		m_player->SetIsOperation(true);
	}
}

