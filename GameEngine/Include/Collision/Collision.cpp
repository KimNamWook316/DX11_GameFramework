#include "Collision.h"

bool CCollision::CollisionBox2DToPoint(CollisionResult& srcResult, CollisionResult& destResult, const Box2DInfo& boxInfo, const Vector2& point)
{
    // 상자의 x, y축에 충돌 사각형의 중심과 점을 이은 벡터를 투영한다.
    // 이 투영된 벡터 둘 모두가 충돌 사각형의 너비 / 2 보다 작다면, 충돌된 것으로 처리한다.
    Vector2 centerToDot = boxInfo.Center - point;
    
    // x축 투영
    Vector2 axis = boxInfo.Axis[0];
    float projLength = abs(centerToDot.Dot(axis));
    
    if (projLength > boxInfo.Length.x)
    {
        return false;
    }

    // y축 투영
    axis = boxInfo.Axis[1];
    projLength = abs(centerToDot.Dot(axis));

    if (projLength > boxInfo.Length.y)
    {
        return false;
    }

    return true;
}
