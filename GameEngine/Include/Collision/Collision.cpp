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
    // �� Box Collider�� Local���� �������� �����Ͽ�, �и����� ���ٸ� �浹�� ������ ó���Ѵ�.
    Vector2 centerToCenter = src.Center - dest.Center;
    
    // src�� Local x�� ����
    Vector2 axis = src.Axis[0];
    
    // �߽ɿ��� �߽��� ���ϴ� ���͸� src�� local�࿡ ������ ����
    float centerProjLegnth = abs(centerToCenter.Dot(axis));

    float proj1;
    float proj2;

    // src�� x������ �����ϰ� �����Ƿ�, src�� ���� ���� Length�� ����.
    proj1 = src.Length.x;
    // dest�� x���� src�� x��� �������� ���� �̻�, y ���������� ���ľ� ��Ȯ�� ���� ���̰� ���´�.
    proj2 = abs(dest.Axis[0].Dot(axis) * dest.Length.x) + abs(dest.Axis[1].Dot(axis) * dest.Length.y);

    // ���� ���̰� ���� ���� �ͺ��� ũ�� == �и����� �����Ѵٴ� ���̹Ƿ� �浹���� �ʾҴٴ� �� 
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
    // ������ x, y�࿡ �浹 �簢���� �߽ɰ� ���� ���� ���͸� �����Ѵ�.
    // �� ������ ���� �� ��ΰ� �浹 �簢���� �ʺ� / 2 ���� �۴ٸ�, �浹�� ������ ó���Ѵ�.
    Vector2 centerToDot = boxInfo.Center - point;
    
    // x�� ����
    Vector2 axis = boxInfo.Axis[0];
    float projLength = abs(centerToDot.Dot(axis));
    
    if (projLength > boxInfo.Length.x)
    {
        return false;
    }

    // y�� ����
    axis = boxInfo.Axis[1];
    projLength = abs(centerToDot.Dot(axis));

    if (projLength > boxInfo.Length.y)
    {
        return false;
    }

    return true;
}
