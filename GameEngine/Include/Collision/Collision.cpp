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
    // 박스끼리 겹치지 않았다면 충돌되지 않음
    if (!CollisionBox2DToBox2D(srcResult, destResult, src, dest.Box))
    {
        return false;
    }

    // 겹치는 부분을 구한다.
    float left = src.Min.x < dest.Min.x ? dest.Min.x : src.Min.x;
    float right = src.Max.x > dest.Max.x ? dest.Max.x : src.Max.x;
    float bottom = src.Min.y > dest.Min.y ? src.Min.y : dest.Min.y;
    float top = src.Max.y < dest.Max.y ? src.Max.y : dest.Max.y;

    // 월드 공간에서의 좌하단 좌표를 구한다.
    Vector2 LB = dest.Box.Center - dest.Box.Length;

    // 인덱스를 구하기 위해, 픽셀 충돌체의 LB기준으로 정렬한다.
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

    // 교집합 구간의 모든 픽셀 검사
    for (int y = (int)top; y < (int)bottom; ++y)
    {
        for (int x = (int)left; x < (int)right; ++x)
        {
            // unsigned char로 이루어진(rgba) 4byte구조 배열이므로 인덱스 이렇게 구함
            int idx = y * (int)dest.Width * 4 + x * 4;

            // 현재 인덱스의 픽셀이 상대방 박스 안에 존재하는지 판단한다.
            // 현재 픽셀의 월드공간에서의 위치를 구한다.
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

    // 겹치는 부분을 구한다.
    float left = src.Min.x < dest.Min.x ? dest.Min.x : src.Min.x;
    float right = src.Max.x > dest.Max.x ? dest.Max.x : src.Max.x;
    float bottom = src.Min.y > dest.Min.y ? src.Min.y : dest.Min.y;
    float top = src.Max.y < dest.Max.y ? src.Max.y : dest.Max.y;

    // 월드 공간에서의 좌하단 좌표를 구한다.
    Vector2 LB = dest.Box.Center - dest.Box.Length;

    // 인덱스를 구하기 위해, 픽셀 충돌체의 LB기준으로 정렬한다.
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

    // 교집합 구간의 모든 픽셀 검사
    for (int y = (int)top; y < (int)bottom; ++y)
    {
        for (int x = (int)left; x < (int)right; ++x)
        {
            // unsigned char로 이루어진(rgba) 4byte구조 배열이므로 인덱스 이렇게 구함
            int idx = y * (int)dest.Width * 4 + x * 4;

            // 현재 인덱스의 픽셀이 상대방 박스 안에 존재하는지 판단한다.
            // 현재 픽셀의 월드공간에서의 위치를 구한다.
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
    
    // 점의 텍스쳐 내에서의 인덱스를 구한다.
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
