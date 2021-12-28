#pragma once

#include "../GameInfo.h"

class CCollision
{
public:
	static bool CollisionBox2DToPoint(CollisionResult& srcResult, CollisionResult& destResult,
		const Box2DInfo& boxInfo, const Vector2& point);
};

