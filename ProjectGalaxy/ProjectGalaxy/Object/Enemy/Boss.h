#pragma once
#include "Enemy.h"
class Boss : public Enemy
{
public:
	Boss();
	virtual ~Boss();

	void Init();
	void Update();
	void Draw();

private:
};

