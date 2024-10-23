#pragma once
#include "Collidable.h"
class Player;

class SeekerLine : public MyEngine::Collidable
{
public:
	SeekerLine(std::vector<Vec3> points, int color);
	virtual ~SeekerLine();

	void Init();
	void Update();
	void Draw();
	Vec3 GetPos() { return m_rigid->GetPos(); }

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);

private:
	std::vector<Vec3> m_points;
	std::shared_ptr<Player> m_player;
	Vec3 m_playerStartPos;


	int m_color;
	int m_hitPointNum;
	int m_speed;
	int m_ratio;
};