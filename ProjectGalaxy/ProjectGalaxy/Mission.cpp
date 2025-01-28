#include "Mission.h"

Mission::Mission()
{
}

Mission::~Mission()
{
}

void Mission::UpDate()
{
	(this->*m_missionUpdate)();
}

void Mission::Draw()
{
	(this->*m_missionDraw)();
}

void Mission::MoveUpdate()
{
}

void Mission::MoveDraw()
{
}
