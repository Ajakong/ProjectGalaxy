﻿#pragma once
#include "../MyLib/Physics/Collidable.h"
class WarpGate : public MyEngine::Collidable
{
public:
	WarpGate(Vec3 pos, Vec3 warpPos, int handle);
	~WarpGate();

	void Init();
	void Update();
	void SetEffectPos();
	void Draw();
	Vec3 GetNowPlanetPos() { return m_nowPlanetPos; }
	
	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider,ColideTag ownTag,ColideTag targetTag);

private:
	Vec3 m_warpPos;
	Vec3 m_nowPlanetPos;
	int m_gaussScreenHandle;
	int m_colorScreenHandle;
	int m_highBrightScreenHandle;
	int m_screenHandle;
};

