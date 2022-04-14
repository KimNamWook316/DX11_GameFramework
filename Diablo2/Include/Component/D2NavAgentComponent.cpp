#include "D2NavAgentComponent.h"
#include "GameObject/GameObject.h"
#include "D2CharacterInfoComponent.h"
#include "../D2Util.h"
#include "Scene/Scene.h"

CD2NavAgentComponent::CD2NavAgentComponent()	:
	mCurOnIdx(0),
	mPrevOnIdx(0),
	mbMoveLinear(false)
{
	SetTypeID<CD2NavAgentComponent>();
}

CD2NavAgentComponent::CD2NavAgentComponent(const CD2NavAgentComponent& com)	:
	CNavAgentComponent(com)
{
	mCurOnIdx = -1;
	mPrevOnIdx = -1;
	mbMoveLinear = false;
}

CD2NavAgentComponent::~CD2NavAgentComponent()
{
}

void CD2NavAgentComponent::Start()
{
	CNavAgentComponent::Start();
	mCharInfo = mObject->FindObjectComponentFromType<CD2CharacterInfoComponent>();
}

void CD2NavAgentComponent::Update(float deltaTime)
{
	if (mUpdateComponent)
	{
		if (!mPathList.empty())
		{
			// 현재 최적 경로 찾지 않고 직선상 이동중인 경우
			if (mbMoveLinear)
			{
				// 앞이 막혀있다면, 길찾기 시작한다.
				if (getIsBlockedFront())
				{
					PathFindTileInfo* info = mScene->GetNavigationManager()->GetNavData()->GetPathFindTile(mObject->GetWorldPos());
					mEndPos = info->Center;
					mPathList.clear();
					findPath();
					return;
				}
			}

			// Isometric 타일에서의 속도 조절
			float moveSpeed = CD2Util::CorrectSpeed(mCharInfo->GetDir(), mCharInfo->GetSpeed());

			Vector3 targetPos = mPathList.front();
			Vector3 pos = mUpdateComponent->GetWorldPos();

			float targetDist = pos.Distance(targetPos);

			Vector3 dir = targetPos - pos;
			dir.Normalize();

			float dist = moveSpeed * deltaTime;

			if (targetDist <= dist)
			{
				mPathList.pop_front();
				dist = targetDist;
			}

			mUpdateComponent->AddWorldPos(dir * dist);

			if (mPathList.empty())
			{
				if (mEndCallBack)
				{
					mEndCallBack();
					mEndCallBack = nullptr;
				}
			}
			else
			{
				if (mFrameCallBack)
				{
					mFrameCallBack();
				}
			}
		}
	}
}

CD2NavAgentComponent* CD2NavAgentComponent::Clone()
{
	return new CD2NavAgentComponent(*this);
}

bool CD2NavAgentComponent::Move(const Vector3& endPos)
{
    if (!mUpdateComponent)
    {
		assert(false);
        return false;
    }

	mEndPos = endPos;

	// 바로 전에 입력되었던 곳이라면 길 찾지 않는다.
    int endIdx = mScene->GetNavigationManager()->GetIndex(endPos);

    if (mPrevEndIdx == endIdx)
    {
        return false;
    }

    mPrevEndIdx = endIdx;

	if (!mScene->GetNavigationManager()->GetNavData()->IsReachableTile(endPos))
	{
		return false;
	}

	// 이동 방향이 막혀있다면, 바로 길을 찾는다.
	if (getIsBlockedFront())
	{
		PathFindTileInfo* info = mScene->GetNavigationManager()->GetNavData()->GetPathFindTile(endPos);
		mEndPos = info->Center;
		return findPath();
	}

	// 이전 경로는 무시한다.
	mPathList.clear();
	mPathList.push_back(endPos);
	mbMoveLinear = true;
	return true;
}

void CD2NavAgentComponent::CancleMove()
{
	mPathList.clear();
}

void CD2NavAgentComponent::onFindPathResult(const std::list<Vector3>& pathList)
{
	CNavAgentComponent::onFindPathResult(pathList);
	mbMoveLinear = false;
}

bool CD2NavAgentComponent::getIsBlockedFront()
{
	CTileMapComponent* navData = mScene->GetNavigationManager()->GetNavData();
	PathFindTileInfo* info = navData->GetPathFindTile(mObject->GetWorldPos());

	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			if (x == 0 && y == 0)
			{
				continue;
			}
			if (navData->GetPathFindTile(info->IndexX + x, info->IndexY + y)->Type == eTileType::Wall)
			{
				return true;
			}
		}
	}
	return false;
}

bool CD2NavAgentComponent::findPath()
{
    return mScene->GetNavigationManager()->FindPath<CD2NavAgentComponent>(this, &CD2NavAgentComponent::onFindPathResult,
        mUpdateComponent->GetWorldPos(), mEndPos);
}
