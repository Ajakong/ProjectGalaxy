#include "SeekerLine.h"
#include"Player.h"
#include"Easing.h"
#include"ColliderSphere.h"

namespace
{
	constexpr int kSpeedMax = 20;
	constexpr int kRadius = 50;
}

SeekerLine::SeekerLine(std::vector<Vec3> points, int color) : Collidable(Priority::StageGimmick, ObjectTag::SeekerLine),
m_hitPointNum(0)
{
	for (auto& point : points)
	{
		m_points.push_back(point);
	}
	m_rigid->SetPos(m_points.front());
	AddCollider(MyEngine::ColliderBase::Kind::Sphere);
	auto item = dynamic_pointer_cast<MyEngine::ColliderSphere>(m_colliders.back());
	item->radius = kRadius;
	SetAntiGravity();
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
	//ポイントの更新

	float lenge = (m_rigid->GetPos() - m_player->GetPos()).Length();
	float ratio = (lenge / m_playerStartPos.Length());
	m_ratio += 0.002f;
	if (m_ratio > 1)
	{
		m_ratio = 1;
	}
	m_player->SetPos(EaseInOut(m_points[m_hitPointNum], m_points[m_hitPointNum + 1], 1, 2) - m_player->GetPos());

	if ((m_player->GetPos() - m_points[m_hitPointNum + 1]).Length() <= 20)//次のポイントに到達したら
	{
		m_hitPointNum++;
		if (m_points[m_hitPointNum] == m_points.back())//次のポイントが最後だったら
		{
			m_player->SetIsOperation(false);
			//プレイヤーをジャンプさせる
			m_player->CommandJump();
			m_hitPointNum = 1;
			m_player = nullptr;
		}
	}
}

void SeekerLine::Draw()
{
	DrawSphere3D(m_points[0].VGet(), kRadius, 8, 0xffff00, 0xffffff, false);
	for (int i = 0; i < m_points.size(); i++)
	{
		if (i <= 0)continue;
		DrawLine3D((m_points.begin() + i-1)->VGet(), (m_points.begin() + i)->VGet(), m_color);
		if ((m_points.begin() + i) == m_points.end())return;
	}
}

void SeekerLine::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{
		m_player = std::dynamic_pointer_cast<Player>(colider);
		m_player->SetIsOperation(true);
	}
}

