#include "Collision.h"
#include "../Component/ColliderBox2D.h"

bool CCollision::CollisionBox2DToBox2D(CColliderBox2D* src, CColliderBox2D* dest)
{
    CollisionResult srcResult;
    CollisionResult destResult;

    if (CollisionBox2DToBox2D(srcResult, destResult, src->GetInfo(), dest->GetInfo()))
    {
        srcResult.Src = src;
        srcResult.Dest = dest;
        destResult.Src = dest;
        destResult.Dest = src;

        src->mResult = srcResult;
        dest->mResult = destResult;

        return true;
    }
    return false;
}

bool CCollision::CollisionBox2DToBox2D(CollisionResult& srcResult, CollisionResult& destResult, const Box2DInfo& src, const Box2DInfo& dest)
{
    // 두 Box Collider의 Local축을 기준으로 투영하여, 분리축이 없다면 충돌한 것으로 처리한다.
    Vector2 centerToCenter = src.Center - dest.Center;
    
    // src의 Local x축 기준
    Vector2 axis = src.Axis[0];
    
    // 중심에서 중심을 향하는 벡터를 src의 local축에 투영한 길이
    float centerProjLegnth = abs(centerToCenter.Dot(axis));

    float proj1;
    float proj2;

    // src의 x축으로 투영하고 있으므로, src의 투영 값은 Length와 같다.
    proj1 = src.Length.x;
    // dest의 x축이 src의 x축과 평행하지 않은 이상, y 투영값까지 합쳐야 정확한 투영 길이가 나온다.
    proj2 = abs(dest.Axis[0].Dot(axis) * dest.Length.x) + abs(dest.Axis[1].Dot(axis) * dest.Length.y);

    // 투영 길이가 둘을 더한 것보다 크다 == 분리축이 존재한다는 뜻이므로 충돌하지 않았다는 뜻 
    if (centerProjLegnth > proj1 + proj2)
    {
        return false;
    }

    axis = src.Axis[1];

    centerProjLegnth = abs(centerToCenter.Dot(axis));

    proj1 = src.Length.y;
    proj2 = abs(dest.Axis[0].Dot(axis) * dest.Length.x) + abs(dest.Axis[1].Dot(axis) * dest.Length.y);

    if (centerProjLegnth > proj1 + proj2)
    {
        return false;
    }
    
    axis = dest.Axis[0];

    centerProjLegnth = abs(centerToCenter.Dot(axis));

    proj1 = dest.Length.x;
    proj2 = abs(src.Axis[0].Dot(axis) * src.Length.x) + abs(src.Axis[1].Dot(axis) * src.Length.y);

    if (centerProjLegnth > proj1 + proj2)
    {
        return false;
    }

    axis = dest.Axis[1];

    centerProjLegnth = abs(centerToCenter.Dot(axis));

    proj1 = dest.Length.y;
    proj2 = abs(src.Axis[0].Dot(axis) * src.Length.x) + abs(src.Axis[1].Dot(axis) * src.Length.y);

    if (centerProjLegnth > proj1 + proj2)
    {
        return false;
    }

    return true;
}

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
