#pragma once
#include "Item.h"
class ClearObject : public Item
{
public:
	ClearObject(Vec3 pos,bool antigravity=false);
	virtual ~ClearObject();

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider,MyEngine::ColliderBase::ColideTag tag);

	void Init();
	void Draw();
};

