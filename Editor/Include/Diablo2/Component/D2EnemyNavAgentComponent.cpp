#include "D2EnemyNavAgentComponent.h"
#include "GameObject/GameObject.h"
#include "D2CharacterInfoComponent.h"
#include "../D2Util.h"
#include "Scene/Scene.h"

CD2EnemyNavAgentComponent::CD2EnemyNavAgentComponent()	:
	mCurOnIdx(0),
	mPrevOnIdx(0),
	mbWait(false)
{
	SetTypeID<CD2EnemyNavAgentComponent>();
}

CD2EnemyNavAgentComponent::CD2EnemyNavAgentComponent(const CD2EnemyNavAgentComponent& com)	:
	CNavAgentComponent(com)
{
	mCurOnIdx = 0;
	mPrevOnIdx = 0;
	mbWait = false;
}

CD2EnemyNavAgentComponent::~CD2EnemyNavAgentComponent()
{
}

void CD2EnemyNavAgentComponent::Start()
{
	CNavAgentComponent::Start();
	mCharInfo = mObject->FindObjectComponentFromType<CD2CharacterInfoComponent>();
}

void CD2EnemyNavAgentComponent::Update(float deltaTime)
{
	if (mUpdateComponent)
	{
		bool bNewTile = false;
		int idx = mScene->GetNavigationManager()->GetIndex(mObject->GetWorldPos());

		// 현재 밟고 있는 타일과, 이전 타일 저장
		if (mCurOnIdx != idx)
		{
			// 현재 밟고 있는 타일은 못 가는 타일로, 이전에 밟은 타일은 갈 수 있는 타일로 변경
			mScene->GetNavigationManager()->ChangeTile(mPrevOnIdx, eTileType::Normal);

			mPrevOnIdx = mCurOnIdx;
			mCurOnIdx = idx;

			mScene->GetNavigationManager()->ChangeTile(mCurOnIdx, eTileType::Wall);
			bNewTile = true;
		}
		else
		{
			// 현재 밟고 있는 타일은 못 가는 타일로, 이전에 밟은 타일은 갈 수 있는 타일로 변경
			if (mCurOnIdx != mPrevOnIdx)
			{
				mScene->GetNavigationManager()->ChangeTile(mPrevOnIdx, eTileType::Normal);
			}

			mScene->GetNavigationManager()->ChangeTile(mCurOnIdx, eTileType::Wall);
			mPrevOnIdx = idx;
			mCurOnIdx = idx;
		}

		if (!mPathList.empty())
		{
			// 대기 상태라면 이동하지 않는다.
			if (mbWait)
			{
				return;
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

CD2EnemyNavAgentComponent* CD2EnemyNavAgentComponent::Clone()
{
	return new CD2EnemyNavAgentComponent(*this);
}

// 매개변수 주어지지 않을 경우, 플레이어 위치 추적해서 이동한다.
bool CD2EnemyNavAgentComponent::Move(const Vector3& endPos)
{
	if (!mUpdateComponent)
    {
		assert(false);
        return false;
    }

	CNavigationManager* navMng = mScene->GetNavigationManager();

	// 이동할 곳이 지정된 경우, 그 곳으로 이동한다.
	if (endPos != Vector3::Zero)
	{
		mEndPos = endPos;
		return navMng->FindPath<CD2EnemyNavAgentComponent>(this, &CD2EnemyNavAgentComponent::onFindPathResult,
			mUpdateComponent->GetWorldPos(), mEndPos);
	}

	int playerIdx = navMng->GetNavData()->GetPathFindTileIndex(mScene->GetPlayerObj()->GetWorldPos());

	// 플레이어가 이전 길찾기 시도때와 같은 타일에 있다면 찾지 않는다.
	if (playerIdx == mPlayerIdx)
	{
		return true;
	}

	mPlayerIdx = playerIdx;

	// 플레이어 주위 갈 수 있는 타일을 찾는다.
	int aroundPlayerIdx = getAroundPlayerTileIndex();

	// 갈 수 있는 타일을 찾지 못했다면 일단 직선 이동한다.
	if (-1 == aroundPlayerIdx)
	{
		mPathList.clear();
		mPathList.push_back(endPos);
		return true;
	}

	if (mCurReserveIdx == aroundPlayerIdx)
	{
		return true;
	}
	else
	{
		// 이전 길찾기 때 플레이어 주위를 예약했다면, 이전에 예약 상태로 변경된 타일을 예약 전 상태로 되돌린다.
		if (eTileType::Reserved == navMng->GetNavData()->GetPathFindTile(mCurReserveIdx)->Type)
		{
			navMng->ChangeTile(mCurReserveIdx, eTileType::Normal);
		}
	}

	// 길찾기 정보 갱신 목록에 현재 타일을 예약 타일로 변경한다.
	mCurReserveIdx = aroundPlayerIdx;
	navMng->ChangeTile(mCurReserveIdx, eTileType::Reserved);

	// 이전 경로는 무시한다.
	mPathList.clear();

	// 예약 타일을 목표 위치로 지정
	mEndPos = navMng->GetNavData()->GetPathFindTile(mCurReserveIdx)->Center;

	// 예약 타일로 향하는 길을 찾는다.
    return mScene->GetNavigationManager()->FindPath<CD2EnemyNavAgentComponent>(this, &CD2EnemyNavAgentComponent::onFindPathResult,
        mUpdateComponent->GetWorldPos(), mEndPos);
}

// 앞이 막힌 경우, 다시 경로 찾을 때 호출하는 함수
bool CD2EnemyNavAgentComponent::MoveBlockedFront()
{
	if (!mUpdateComponent)
    {
		assert(false);
        return false;
    }

	CNavigationManager* navMng = mScene->GetNavigationManager();

	int playerIdx = navMng->GetNavData()->GetPathFindTileIndex(mScene->GetPlayerObj()->GetWorldPos());

	// 플레이어가 이전 길찾기 시도때와 같은 타일에 있다면 찾지 않는다.
	if (playerIdx != mPlayerIdx)
	{
		mPlayerIdx = playerIdx;

		// 플레이어 주위 갈 수 있는 타일을 찾는다.
		int aroundPlayerIdx = getAroundPlayerTileIndex();

		// 갈 수 있는 타일을 찾지 못했다면 일단 직선 이동한다.
		if (-1 == aroundPlayerIdx)
		{
			mEndPos = navMng->GetNavData()->GetPathFindTile(mPlayerIdx)->Center;
			mPathList.clear();
			mPathList.push_back(mEndPos);
			return true;
		}

		// 이전 길찾기 때 플레이어 주위를 예약했다면, 이전에 예약 상태로 변경된 타일을 예약 전 상태로 되돌린다.
		if (eTileType::Reserved == navMng->GetNavData()->GetPathFindTile(mCurReserveIdx)->Type)
		{
			navMng->ChangeTile(mCurReserveIdx, eTileType::Normal);
		}

		// 길찾기 정보 갱신 목록에 현재 타일을 예약 타일로 변경한다.
		mCurReserveIdx = aroundPlayerIdx;
		navMng->ChangeTile(mCurReserveIdx, eTileType::Reserved);
	}

	// 이전 경로는 무시한다.
	mPathList.clear();

	// 예약 타일을 목표 위치로 지정
	mEndPos = navMng->GetNavData()->GetPathFindTile(mCurReserveIdx)->Center;

	// 예약 타일로 향하는 길을 찾는다.
    return mScene->GetNavigationManager()->FindPath<CD2EnemyNavAgentComponent>(this, &CD2EnemyNavAgentComponent::onFindPathResult,
        mUpdateComponent->GetWorldPos(), mEndPos);
}

void CD2EnemyNavAgentComponent::CancleMove()
{
	mPathList.clear();
}

void CD2EnemyNavAgentComponent::OnDie()
{
	CNavigationManager* navMng = mScene->GetNavigationManager();
	navMng->ChangeTile(mCurReserveIdx, eTileType::Normal);
	navMng->ChangeTile(mCurOnIdx, eTileType::Normal);
	mPathList.clear();
}

void CD2EnemyNavAgentComponent::onFindPathResult(const std::list<Vector3>& pathList)
{
	CNavAgentComponent::onFindPathResult(pathList);
}

bool CD2EnemyNavAgentComponent::getIsBlockedFront()
{
	CTileMapComponent* navData = mScene->GetNavigationManager()->GetNavData();
	PathFindTileInfo* info = navData->GetPathFindTile(mCurOnIdx);

	// 현재 위치에서 목표 지점을 향하는 벡터
	Vector3 moveDirection = mEndPos - mObject->GetWorldPos();

	Vector3 upVec =  Vector3(0.f, info->Size.y / 2.f, 0.f);
	Vector3 rightVec = Vector3(info->Size.x / 2.f, 0.f, 0.f);
	Vector3 downVec = Vector3(0.f, -info->Size.y / 2.f, 0.f);
	Vector3 leftVec = Vector3(-info->Size.x / 2.f, 0.f, 0.f);

	// ccw 계산을 통해, 직선상의 경로가 타일의 어느 면과 교차하는지 판별한다.
	if (moveDirection.x > 0 && moveDirection.y > 0)
	{
		float crossUp = moveDirection.Cross(upVec).z;
		float crossRight = moveDirection.Cross(rightVec).z;

		if (crossUp == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx + navData->GetPathFindTileCountX() + 1))
			{
				return true;
			}
		}
		// 오른쪽 꼭지점
		else if (crossRight == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx - navData->GetPathFindTileCountX() + 1))
			{
				return true;
			}
		}

		float crossUpRight = crossUp * crossRight;

		// 진행방향의 다음 타일이 오른쪽 위인 경우
		if (crossUpRight < 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx + 1))
			{
				return true;
			}
		}
		return false;
	}

	// 왼쪽 위
	else if (moveDirection.x < 0 && moveDirection.y > 0)
	{
		float crossUp = moveDirection.Cross(upVec).z;
		float crossLeft = moveDirection.Cross(leftVec).z;

		// 위쪽 꼭지점에 닿을 경우
		if (crossUp == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx + navData->GetPathFindTileCountX() + 1))
			{
				return true;
			}
		}
		// 왼
		else if (crossLeft == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx + navData->GetPathFindTileCountX() - 1))
			{
				return true;
			}
		}

		float crossLeftUp = crossLeft * crossUp;

		// 왼쪽 위
		if (crossLeftUp < 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx + navData->GetPathFindTileCountX()))
			{
				return true;
			}
		}
		return false;
	}

	// 왼쪽 아래
	else if (moveDirection.x < 0 && moveDirection.y < 0)
	{
		float crossDown = moveDirection.Cross(downVec).z;
		float crossLeft = moveDirection.Cross(leftVec).z;

		// 아래
		if (crossDown == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx - navData->GetPathFindTileCountX() - 1))
			{
				return true;
			}
		}
		// 왼
		else if (crossLeft == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx + navData->GetPathFindTileCountX() - 1))
			{
				return true;
			}
		}

		float crossDownLeft = crossDown * crossLeft;

		// 왼쪽 아래
		if (crossDownLeft < 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx - 1))
			{
				return true;
			}
		}
		return false;
	}
	else if (moveDirection.x > 0 && moveDirection.y < 0)
	{
		float crossRight = moveDirection.Cross(rightVec).z;
		float crossDown = moveDirection.Cross(downVec).z;

		// 오른쪽 꼭지점
		if (crossRight == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx - navData->GetPathFindTileCountX() + 1))
			{
				return true;
			}
		}
		// 아래
		else if (crossDown == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx - navData->GetPathFindTileCountX() - 1))
			{
				return true;
			}
		}

		float crossRightDown = crossRight * crossDown;
		// 오른쪽 아래
		if (crossRightDown < 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx - navData->GetPathFindTileCountX()))
			{
				return true;
			}
		}
		return false;
	}
	return false;
}

int CD2EnemyNavAgentComponent::getAroundPlayerTileIndex()
{
	CTileMapComponent* tileMap = mScene->GetNavigationManager()->GetNavData();
	Vector3 playerPos = mScene->GetPlayerObj()->GetWorldPos();

	int playerIdxX = tileMap->GetPathFindTileIndexX(playerPos);
	int playerIdxY = tileMap->GetPathFindTileIndexY(playerPos);
	int playerIdx = tileMap->GetPathFindTileIndex(playerPos);

	// 플레이어 주위로 이미 다른 AI가 예약하지 않은 타일이 있는지 찾아서 반환한다.
	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			int idx = ((playerIdxY + y) * tileMap->GetPathFindTileCountX()) + playerIdxX + x;

			if (idx == playerIdx)
			{
				continue;
			}

			if (eTileType::Normal == tileMap->GetPathFindTile(idx)->Type)
			{
				return idx;
			}
		}
	}

	// 범위를 넓혀 예약하지 않은 타일을 찾는다.
	int tryCount = 0;
	int searchRangeMin = -2;
	int searchRangeMax = 2;

	// 3번 이상 실패할 경우 루프에서 벗어난다.
	while (tryCount < 3)
	{
		++tryCount;

		// 사각형 범위 위 아래 탐색
		for (int x = searchRangeMin; x <= searchRangeMax; ++x)
		{
			int idxX = playerIdxX + x;
			int idxY = playerIdxY + searchRangeMax;
			
			// 위쪽 가장자리 탐색
			if (eTileType::Normal == tileMap->GetPathFindTile(idxX, idxY)->Type)
			{
				return (idxY * tileMap->GetPathFindTileCountX()) + idxX;
			}

			// 아래쪽 가장자리 탐색
			idxY = playerIdxY + searchRangeMin;
			if (eTileType::Normal == tileMap->GetPathFindTile(idxX, idxY)->Type)
			{
				return (idxY * tileMap->GetPathFindTileCountX()) + idxX;
			}
		}

		// 사각형 범위 왼, 오른쪽 탐색
		for (int y = searchRangeMin + 1; y <= searchRangeMax - 1; ++y)
		{
			int idxX = playerIdxX + searchRangeMin;
			int idxY = playerIdxY + y;
			
			// 왼쪽 가장자리 탐색
			if (eTileType::Normal == tileMap->GetPathFindTile(idxX, idxY)->Type)
			{
				return (idxY * tileMap->GetPathFindTileCountX()) + idxX;
			}

			// 오른쪽 가장자리 탐색
			idxX = playerIdxX + searchRangeMax;
			if (eTileType::Normal == tileMap->GetPathFindTile(idxX, idxY)->Type)
			{
				return (idxY * tileMap->GetPathFindTileCountX()) + idxX;
			}
		}

		++searchRangeMax;
		--searchRangeMin;
	}

	// 탐색 실패한 경우
	return -1;
}
