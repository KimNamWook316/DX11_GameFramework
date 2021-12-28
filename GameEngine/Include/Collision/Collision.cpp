#include "Collision.h"

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
