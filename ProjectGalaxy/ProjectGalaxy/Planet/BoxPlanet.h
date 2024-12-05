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

	virtual Vec3 GravityEffect(std::shared_ptr<Collidable> obj);
	virtual Vec3 FrictionEffect(std::shared_ptr<Collidable> obj);
	virtual Vec3 GetNormVec(Vec3 pos);
	virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider,ColideTag ownTag,ColideTag targetTag);
	virtual void OnTriggerExit(std::shared_ptr<Collidable> colider,ColideTag ownTag,ColideTag targetTag);
private:
	Vec3 norm;
	Vec3 size;
	int m_color;
};

