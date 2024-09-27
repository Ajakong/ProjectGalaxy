#pragma once
#include "Planet.h"
class BoxPlanet :  public Planet
{
public:
	BoxPlanet(Vec3 pos, int color);
	virtual ~BoxPlanet();

	void Init();
	void Update();
	void Draw();

	virtual void GravityEffect();
	virtual Vec3 GetNormVec(Vec3 pos);
	virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider);
	virtual void OnTriggerExit(std::shared_ptr<Collidable> colider);
private:

	int m_color;
};

