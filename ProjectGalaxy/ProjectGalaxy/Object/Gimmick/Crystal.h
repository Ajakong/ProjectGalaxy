#pragma once
#include "Collidable.h"
class Crystal : public MyEngine::Collidable
{
public:
	Crystal(Vec3 pos,Vec3 norm,Vec3 size);
	virtual ~Crystal();

	void Init();
	void Update();
	void Draw();
private:
	int m_Hp;
	Vec3 m_size;
};

