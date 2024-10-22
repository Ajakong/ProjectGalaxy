#pragma once
#include "Item.h"
class ClearObject : public Item
{
public:
	ClearObject(Vec3 pos,bool antigravity=false);
	virtual ~ClearObject();

	virtual void OnCollideEnter(std::shared_ptr<Collidable> colider, int myIndex, int partnerIndex);

	void Init();
	void Draw();
};

