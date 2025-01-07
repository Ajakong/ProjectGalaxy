﻿#pragma once
#include<cmath>
#include<list>
#include"ColliderLine3D.h"
#include"Collidable.h"
class Player;

class StampImpact :public MyEngine::Collidable 
{
public:
	StampImpact(Vec3 pos,float planetRadius,Vec3 dir,ObjectTag tag,float speed=1.f);
	virtual ~StampImpact();

	void Init();
	void Update();
	void Draw();

    bool GetDeleteFlag() { return m_deleteFlag; }
	virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag);
    //メンバ関数ポインタ
    using impactState_t = void(StampImpact::*)();
    impactState_t m_impactUpdate;
private:
    void Draw3DCircle(Vec3 center, float radius, int num_segments);
    void ExpansionUpdate();
    void ReductionUpdate();

	Vec3 m_dir;
	std::vector<std::shared_ptr<MyEngine::ColliderLine3D>>m_lines;

	int m_lifeTime;
	float m_radiusMax;
	float m_nowRadius;
	float m_speed;

    bool m_deleteFlag;

	

};