#include "Collision.h"
#include "../Component/ColliderBox2D.h"
#include "../Component/ColliderCircle.h"
#include "../Component/ColliderPixel.h"

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

bool CCollision::CollisionCircleToCircle(CColliderCircle* src, CColliderCircle* dest)
{
    CollisionResult srcResult;
    CollisionResult destResult;

    if (CollisionCircleToCircle(srcResult, destResult, src->GetInfo(), dest->GetInfo()))
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

bool CCollision::CollisionBox2DToCircle(CColliderBox2D* src, CColliderCircle* dest)
{
    CollisionResult srcResult;
    CollisionResult destResult;

    if (CollisionBox2DToCircle(srcResult, destResult, src->GetInfo(), dest->GetInfo()))
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

bool CCollision::CollisionBox2DToPixel(CColliderBox2D* src, CColliderPixel* dest)
{
    CollisionResult srcResult;
    CollisionResult destResult;

    if (CollisionBox2DToPixel(srcResult, destResult, src->GetInfo(), dest->GetInfo()))
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

bool CCollision::CollisionCircleToPixel(CColliderCircle* src, CColliderPixel* dest)
{
    CollisionResult srcResult;
    CollisionResult destResult;

    if (CollisionCircleToPixel(srcResult, destResult, src->GetInfo(), dest->GetInfo()))
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

bool CCollision::CollisionCircleToCircle(CollisionResult& srcResult, CollisionResult& destResult, const CircleInfo& src, const CircleInfo& dest)
{
    float dist = src.Center.Distance(dest.Center);

    return dist <= src.Radius + dest.Radius;
}

bool CCollision::CollisionBox2DToCircle(CollisionResult& srcResult, CollisionResult& destResult, const Box2DInfo& src, const CircleInfo& dest)
{
    Vector2 centerToCenter = src.Center - dest.Center;

    Vector2 axis = src.Axis[0];
    float centerProjLength = abs(centerToCenter.Dot(axis));
    
    float proj1, proj2;
    proj1 = src.Length.x;
    proj2 = dest.Radius;

    if (proj1 + proj2 < centerProjLength)
    {
        return false;
    }

    axis = src.Axis[1];
    proj1 = src.Length.y;
    centerProjLength = abs(centerToCenter.Dot(axis));
    if (proj1 + proj2 < centerProjLength)
    {
        return false;
    }

    axis = centerToCenter;
    axis.Normalize();
    centerProjLength = centerToCenter.Length();
    proj1 = abs(src.Axis[0].Dot(axis) * src.Length.x) + abs(src.Axis[1].Dot(axis) * src.Length.y);
    if (proj1 + proj2 < centerProjLength)
    {
        return false;
    }

    return true;
}

bool CCollision::CollisionBox2DToPixel(CollisionResult& srcResult, CollisionResult& destResult, const Box2DInfo& src, const PixelInfo& dest)
{
    // �ڽ����� ��ġ�� �ʾҴٸ� �浹���� ����
    if (!CollisionBox2DToBox2D(srcResult, destResult, src, dest.Box))
    {
        return false;
    }

    // ��ġ�� �κ��� ���Ѵ�.
    float left = src.Min.x < dest.Min.x ? dest.Min.x : src.Min.x;
    float right = src.Max.x > dest.Max.x ? dest.Max.x : src.Max.x;
    float bottom = src.Min.y > dest.Min.y ? src.Min.y : dest.Min.y;
    float top = src.Max.y < dest.Max.y ? src.Max.y : dest.Max.y;

    // ���� ���������� ���ϴ� ��ǥ�� ���Ѵ�.
    Vector2 LB = dest.Box.Center - dest.Box.Length;

    // �ε����� ���ϱ� ����, �ȼ� �浹ü�� LB�������� �����Ѵ�.
    left -= LB.x;
    right -= LB.x;
    bottom -= LB.y;
    top -= LB.y;

    left = left < 0.f ? 0.f : left;
    bottom = bottom < 0.f ? 0.f : bottom;
    right = right >= (float)dest.Width ? (float)dest.Width - 1.f : right;
    top = top >= (float)dest.Height ? (float)dest.Height - 1.f : top;

    top = dest.Height - top;
    bottom = dest.Height - bottom;

    bool bCollide = false;

    // ������ ������ ��� �ȼ� �˻�
    for (int y = (int)top; y < (int)bottom; ++y)
    {
        for (int x = (int)left; x < (int)right; ++x)
        {
            // unsigned char�� �̷����(rgba) 4byte���� �迭�̹Ƿ� �ε��� �̷��� ����
            int idx = y * (int)dest.Width * 4 + x * 4;

            // ���� �ε����� �ȼ��� ���� �ڽ� �ȿ� �����ϴ��� �Ǵ��Ѵ�.
            // ���� �ȼ��� ������������� ��ġ�� ���Ѵ�.
            Vector2 pixelWorldPos = LB + Vector2((float)x, (float)dest.Height - (float)y);
            if (!CollisionBox2DToPoint(srcResult, destResult, src, pixelWorldPos))
            {
                continue;
            }

            switch (dest.Type)
            {
            case ePixelCollisionType::ColorConfirm:
                if (dest.Pixel[idx] == dest.Color[0] &&
                    dest.Pixel[idx + 1] == dest.Color[1] &&
                    dest.Pixel[idx + 2] == dest.Color[2])
                {
                    bCollide = true;
                    break;
                }
                else
                {
                    continue;
                }
            case ePixelCollisionType::ColorIgnore:
                if (dest.Pixel[idx] == dest.Color[0] &&
                    dest.Pixel[idx + 1] == dest.Color[1] &&
                    dest.Pixel[idx + 2] == dest.Color[2])
                {
                    continue;
                }
                else
                {
                    bCollide = true;
                    break;
                }
            case ePixelCollisionType::AlphaConfirm:
                if (dest.Pixel[3] == dest.Color[3])
                {
                    bCollide = true;
                    break;
                }
                else
                {
                    continue;
                }
            case ePixelCollisionType::AlphaIgnore:
                if (dest.Pixel[3] == dest.Color[3])
                {
                    continue;
                }
                else
                {
                    bCollide = true;
                    break;
                }
            default:
                assert(false);
                return false;
            }

            if (bCollide)
            {
                break;
            }
        }
        if (bCollide)
        {
            break;
        }
    }
    return bCollide;
}

bool CCollision::CollisionCircleToPixel(CollisionResult& srcResult, CollisionResult& destResult, const CircleInfo& src, const PixelInfo& dest)
{
    if (!CollisionBox2DToCircle(destResult, srcResult, dest.Box, src))
    {
        return false;
    }

    // ��ġ�� �κ��� ���Ѵ�.
    float left = src.Min.x < dest.Min.x ? dest.Min.x : src.Min.x;
    float right = src.Max.x > dest.Max.x ? dest.Max.x : src.Max.x;
    float bottom = src.Min.y > dest.Min.y ? src.Min.y : dest.Min.y;
    float top = src.Max.y < dest.Max.y ? src.Max.y : dest.Max.y;

    // ���� ���������� ���ϴ� ��ǥ�� ���Ѵ�.
    Vector2 LB = dest.Box.Center - dest.Box.Length;

    // �ε����� ���ϱ� ����, �ȼ� �浹ü�� LB�������� �����Ѵ�.
    left -= LB.x;
    right -= LB.x;
    bottom -= LB.y;
    top -= LB.y;

    left = left < 0.f ? 0.f : left;
    bottom = bottom < 0.f ? 0.f : bottom;
    right = right >= (float)dest.Width ? (float)dest.Width - 1.f : right;
    top = top >= (float)dest.Height ? (float)dest.Height - 1.f : top;

    top = dest.Height - top;
    bottom = dest.Height - bottom;

    bool bCollide = false;

    // ������ ������ ��� �ȼ� �˻�
    for (int y = (int)top; y < (int)bottom; ++y)
    {
        for (int x = (int)left; x < (int)right; ++x)
        {
            // unsigned char�� �̷����(rgba) 4byte���� �迭�̹Ƿ� �ε��� �̷��� ����
            int idx = y * (int)dest.Width * 4 + x * 4;

            // ���� �ε����� �ȼ��� ���� �ڽ� �ȿ� �����ϴ��� �Ǵ��Ѵ�.
            // ���� �ȼ��� ������������� ��ġ�� ���Ѵ�.
            Vector2 pixelWorldPos = LB + Vector2((float)x, (float)dest.Height - (float)y);
            if (!CollisionCircleToPoint(srcResult, destResult, src, pixelWorldPos))
            {
                continue;
            }

            switch (dest.Type)
            {
            case ePixelCollisionType::ColorConfirm:
                if (dest.Pixel[idx] == dest.Color[0] &&
                    dest.Pixel[idx + 1] == dest.Color[1] &&
                    dest.Pixel[idx + 2] == dest.Color[2])
                {
                    bCollide = true;
                    break;
                }
                else
                {
                    continue;
                }
            case ePixelCollisionType::ColorIgnore:
                if (dest.Pixel[idx] == dest.Color[0] &&
                    dest.Pixel[idx + 1] == dest.Color[1] &&
                    dest.Pixel[idx + 2] == dest.Color[2])
                {
                    continue;
                }
                else
                {
                    bCollide = true;
                    break;
                }
            case ePixelCollisionType::AlphaConfirm:
                if (dest.Pixel[3] == dest.Color[3])
                {
                    bCollide = true;
                    break;
                }
                else
                {
                    continue;
                }
            case ePixelCollisionType::AlphaIgnore:
                if (dest.Pixel[3] == dest.Color[3])
                {
                    continue;
                }
                else
                {
                    bCollide = true;
                    break;
                }
            default:
                assert(false);
                return false;
            }

            if (bCollide)
            {
                break;
            }
        }
        if (bCollide)
        {
            break;
        }
    }
    return bCollide;
    
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

bool CCollision::CollisionCircleToPoint(CollisionResult& srcResult, CollisionResult& destResult, const CircleInfo& circleInfo, const Vector2& point)
{
    float dist = circleInfo.Center.Distance(point);
    return dist <= circleInfo.Radius;
}

bool CCollision::CollisionPixelToPoint(CollisionResult& srcResult, CollisionResult& destResult, const PixelInfo& pixelInfo, const Vector2& point)
{
    if (!CollisionBox2DToPoint(srcResult, destResult, pixelInfo.Box, point))
    {
        return false;
    }
    
    // ���� �ؽ��� �������� �ε����� ���Ѵ�.
    Vector2 LB = pixelInfo.Box.Center - pixelInfo.Box.Length;
    Vector2 pointTexturePos = point - LB;
    pointTexturePos.y = pixelInfo.Height - pointTexturePos.y;
    pointTexturePos.y = pointTexturePos.y >= pixelInfo.Height ? pixelInfo.Height - 1 : pointTexturePos.y;

    int idx = (int)pointTexturePos.y * pixelInfo.Width * 4 + (int)pointTexturePos.x * 4;

    switch (pixelInfo.Type)
    {
    case ePixelCollisionType::ColorConfirm:
        if (pixelInfo.Pixel[idx] == pixelInfo.Color[0] &&
            pixelInfo.Pixel[idx + 1] == pixelInfo.Color[1] &&
            pixelInfo.Pixel[idx + 2] == pixelInfo.Color[2])
        {
            return true;
        }
        else
        {
            return false;
        }
    case ePixelCollisionType::ColorIgnore:
        if (pixelInfo.Pixel[idx] == pixelInfo.Color[0] &&
            pixelInfo.Pixel[idx + 1] == pixelInfo.Color[1] &&
            pixelInfo.Pixel[idx + 2] == pixelInfo.Color[2])
        {
            return false;
        }
        else
        {
            return true;
        }
    case ePixelCollisionType::AlphaConfirm:
        if (pixelInfo.Pixel[idx + 3] == pixelInfo.Color[3])
        {
            return true;
        }
        else
        {
            return false;
        }
    case ePixelCollisionType::AlphaIgnore:
        if (pixelInfo.Pixel[idx + 3] == pixelInfo.Color[3])
        {
            return false;
        }
        else
        {
            return true;
        }
    default:
        assert(false);
        return false;
    }
    return false;
}
