#pragma once
#include "ColliderBase.h"

namespace MyEngine
{
	class ColliderSphere : public ColliderBase
	{
	public:
		ColliderSphere();

		float GetRadius()const { return radius; }

	public:
		float radius;
	};
}

