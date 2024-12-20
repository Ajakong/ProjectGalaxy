#pragma once
#include"Vec3.h"
#include<cmath>
class StampImpact
{
public:
	StampImpact(Vec3 pos,float planetRadius,Vec3 dir);
	virtual ~StampImpact();

	void Init();
	void Update();
	void Draw();

    bool GetDeleteFlag() { return m_deleteFlag; }

    //メンバ関数ポインタ
    using impactState_t = void(StampImpact::*)();
    impactState_t m_impactUpdate;
private:
    void Draw3DCircle(Vec3 center, float radius, int num_segments);
    void ExpansionUpdate();
    void ReductionUpdate();

	Vec3 m_pos;
	Vec3 m_dir;

	float m_radiusMax;
	float m_nowRadius;

    bool m_deleteFlag;

};