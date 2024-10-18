#include "SeekerLine.h"
#include"Player.h"
SeekerLine::SeekerLine(std::vector<Vec3> points, int color) : Collidable(Priority::Static,ObjectTag::SeekerLine)
{
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
}

void SeekerLine::Draw()
{
	for (int i = 0; i < m_points.size(); i++)
	{
		DrawLine3D((m_points.begin() + i)->VGet(), (m_points.begin() + i + 1)->VGet(), m_color);
		if ((m_points.begin() + i + 1) == m_points.end())return;
	}
}

void SeekerLine::OnCollideEnter(std::shared_ptr<Collidable> colider)
{
	if (colider->GetTag() == ObjectTag::Player)
	{

	}
}
