#pragma once

#include "../GameInfo.h"

class CCollision
{
public:
	static bool CollisionBox2DToBox2D(class CColliderBox2D* src, class CColliderBox2D* dest);
	static bool CollisionCircleToCircle(class CColliderCircle* src, class CColliderCircle* dest);
	static bool CollisionBox2DToCircle(class CColliderBox2D* src, class CColliderCircle* dest);
	static bool CollisionBox2DToPixel(class CColliderBox2D* src, class CColliderPixel* dest);
	static bool CollisionCircleToPixel(class CColliderCircle* src, class CColliderPixel* dest);

	static bool CollisionBox2DToBox2D(CollisionResult& srcResult, CollisionResult& destResult,
		const Box2DInfo& src, const Box2DInfo& dest);
	static bool CollisionCircleToCircle(CollisionResult& srcResult, CollisionResult& destResult,
		const CircleInfo& src, const CircleInfo& dest);
	static bool CollisionBox2DToCircle(CollisionResult& srcResult, CollisionResult& destResult,
		const Box2DInfo& src, const CircleInfo& dest);
	static bool CollisionBox2DToPixel(CollisionResult& srcResult, CollisionResult& destResult,
		const Box2DInfo& src, const PixelInfo& dest);
	static bool CollisionCircleToPixel(CollisionResult& srcResult, CollisionResult& destResult,
		const CircleInfo& src, const PixelInfo& dest);
	
	static bool CollisionBox2DToPoint(CollisionResult& srcResult, CollisionResult& destResult,
		const Box2DInfo& boxInfo, const Vector2& point);
	static bool CollisionCircleToPoint(CollisionResult& srcResult, CollisionResult& destResult,
		const CircleInfo& circleInfo, const Vector2& point);
	static bool CollisionPixelToPoint(CollisionResult& srcResult, CollisionResult& destResult,
		const PixelInfo& pixelInfo, const Vector2& point);

	// 이미 충돌처리 되고, 겹치는 범위를 얻을 때 사용
	static Box2DInfo GetOverlapAreaAABB(const Box2DInfo& src, const Box2DInfo& dst);
};

