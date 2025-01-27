#pragma once
#include "Item.h"
class FullPowerDropItem : public Item
{
public:
	FullPowerDropItem(Vec3 pos, bool antiGravity = false);
	virtual ~FullPowerDropItem();

	virtual void OnTriggerEnter(std::shared_ptr<Collidable> colider, ColideTag ownTag, ColideTag targetTag);
	void Draw();
};

