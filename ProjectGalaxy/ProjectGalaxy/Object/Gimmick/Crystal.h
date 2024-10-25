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

	bool IsDestroy();

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider);
private:
	int m_Hp;
	bool m_destroyFlag;
	Vec3 m_size;

};

