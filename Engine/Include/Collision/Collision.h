#pragma once

#include "../GameInfo.h"

class CCollision
{
public:
	// Box2D To Box2D
	static bool CollisionBox2DToBox2D(class CColliderBox2D* src, class CColliderBox2D* dest);
	static bool CollisionBox2DToBox2D(CollisionResult& srcResult, CollisionResult& destResult,
		const Box2DInfo& src, const Box2DInfo& dest);
	
	// Box2D To Point
	static bool CollisionBox2DToPoint(CollisionResult& srcResult, CollisionResult& destResult,
		const Box2DInfo& boxInfo, const Vector2& point);
};

