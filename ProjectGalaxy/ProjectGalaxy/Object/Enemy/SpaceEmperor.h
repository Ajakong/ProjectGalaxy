#pragma once
#include "Enemy.h"
class SpaceEmperor : public Enemy
{
public:
	SpaceEmperor(Vec3 pos);
	virtual ~SpaceEmperor();

	void Init();
	void Update();
	void SetMatrix();
	void Draw();

	//メンバ関数ポインタ
	using emperorState_t = void(SpaceEmperor::*)();
	emperorState_t m_update;
private:

	void IntroUpdate();
	void IdleUpdate();
	void AttackUpdate();
	void HitUpdate();

};

