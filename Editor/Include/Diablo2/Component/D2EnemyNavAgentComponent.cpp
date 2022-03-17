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

		// ���� ��� �ִ� Ÿ�ϰ�, ���� Ÿ�� ����
		if (mCurOnIdx != idx)
		{
			// ���� ��� �ִ� Ÿ���� �� ���� Ÿ�Ϸ�, ������ ���� Ÿ���� �� �� �ִ� Ÿ�Ϸ� ����
			mScene->GetNavigationManager()->ChangeTile(mPrevOnIdx, eTileType::Normal);

			mPrevOnIdx = mCurOnIdx;
			mCurOnIdx = idx;

			mScene->GetNavigationManager()->ChangeTile(mCurOnIdx, eTileType::Wall);
			bNewTile = true;
		}
		else
		{
			// ���� ��� �ִ� Ÿ���� �� ���� Ÿ�Ϸ�, ������ ���� Ÿ���� �� �� �ִ� Ÿ�Ϸ� ����
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
			// ��� ���¶�� �̵����� �ʴ´�.
			if (mbWait)
			{
				return;
			}

			// Isometric Ÿ�Ͽ����� �ӵ� ����
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

// �Ű����� �־����� ���� ���, �÷��̾� ��ġ �����ؼ� �̵��Ѵ�.
bool CD2EnemyNavAgentComponent::Move(const Vector3& endPos)
{
	if (!mUpdateComponent)
    {
		assert(false);
        return false;
    }

	CNavigationManager* navMng = mScene->GetNavigationManager();

	// �̵��� ���� ������ ���, �� ������ �̵��Ѵ�.
	if (endPos != Vector3::Zero)
	{
		mEndPos = endPos;
		return navMng->FindPath<CD2EnemyNavAgentComponent>(this, &CD2EnemyNavAgentComponent::onFindPathResult,
			mUpdateComponent->GetWorldPos(), mEndPos);
	}

	int playerIdx = navMng->GetNavData()->GetPathFindTileIndex(mScene->GetPlayerObj()->GetWorldPos());

	// �÷��̾ ���� ��ã�� �õ����� ���� Ÿ�Ͽ� �ִٸ� ã�� �ʴ´�.
	if (playerIdx == mPlayerIdx)
	{
		return true;
	}

	mPlayerIdx = playerIdx;

	// �÷��̾� ���� �� �� �ִ� Ÿ���� ã�´�.
	int aroundPlayerIdx = getAroundPlayerTileIndex();

	// �� �� �ִ� Ÿ���� ã�� ���ߴٸ� �ϴ� ���� �̵��Ѵ�.
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
		// ���� ��ã�� �� �÷��̾� ������ �����ߴٸ�, ������ ���� ���·� ����� Ÿ���� ���� �� ���·� �ǵ�����.
		if (eTileType::Reserved == navMng->GetNavData()->GetPathFindTile(mCurReserveIdx)->Type)
		{
			navMng->ChangeTile(mCurReserveIdx, eTileType::Normal);
		}
	}

	// ��ã�� ���� ���� ��Ͽ� ���� Ÿ���� ���� Ÿ�Ϸ� �����Ѵ�.
	mCurReserveIdx = aroundPlayerIdx;
	navMng->ChangeTile(mCurReserveIdx, eTileType::Reserved);

	// ���� ��δ� �����Ѵ�.
	mPathList.clear();

	// ���� Ÿ���� ��ǥ ��ġ�� ����
	mEndPos = navMng->GetNavData()->GetPathFindTile(mCurReserveIdx)->Center;

	// ���� Ÿ�Ϸ� ���ϴ� ���� ã�´�.
    return mScene->GetNavigationManager()->FindPath<CD2EnemyNavAgentComponent>(this, &CD2EnemyNavAgentComponent::onFindPathResult,
        mUpdateComponent->GetWorldPos(), mEndPos);
}

// ���� ���� ���, �ٽ� ��� ã�� �� ȣ���ϴ� �Լ�
bool CD2EnemyNavAgentComponent::MoveBlockedFront()
{
	if (!mUpdateComponent)
    {
		assert(false);
        return false;
    }

	CNavigationManager* navMng = mScene->GetNavigationManager();

	int playerIdx = navMng->GetNavData()->GetPathFindTileIndex(mScene->GetPlayerObj()->GetWorldPos());

	// �÷��̾ ���� ��ã�� �õ����� ���� Ÿ�Ͽ� �ִٸ� ã�� �ʴ´�.
	if (playerIdx != mPlayerIdx)
	{
		mPlayerIdx = playerIdx;

		// �÷��̾� ���� �� �� �ִ� Ÿ���� ã�´�.
		int aroundPlayerIdx = getAroundPlayerTileIndex();

		// �� �� �ִ� Ÿ���� ã�� ���ߴٸ� �ϴ� ���� �̵��Ѵ�.
		if (-1 == aroundPlayerIdx)
		{
			mEndPos = navMng->GetNavData()->GetPathFindTile(mPlayerIdx)->Center;
			mPathList.clear();
			mPathList.push_back(mEndPos);
			return true;
		}

		// ���� ��ã�� �� �÷��̾� ������ �����ߴٸ�, ������ ���� ���·� ����� Ÿ���� ���� �� ���·� �ǵ�����.
		if (eTileType::Reserved == navMng->GetNavData()->GetPathFindTile(mCurReserveIdx)->Type)
		{
			navMng->ChangeTile(mCurReserveIdx, eTileType::Normal);
		}

		// ��ã�� ���� ���� ��Ͽ� ���� Ÿ���� ���� Ÿ�Ϸ� �����Ѵ�.
		mCurReserveIdx = aroundPlayerIdx;
		navMng->ChangeTile(mCurReserveIdx, eTileType::Reserved);
	}

	// ���� ��δ� �����Ѵ�.
	mPathList.clear();

	// ���� Ÿ���� ��ǥ ��ġ�� ����
	mEndPos = navMng->GetNavData()->GetPathFindTile(mCurReserveIdx)->Center;

	// ���� Ÿ�Ϸ� ���ϴ� ���� ã�´�.
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

	// ���� ��ġ���� ��ǥ ������ ���ϴ� ����
	Vector3 moveDirection = mEndPos - mObject->GetWorldPos();

	Vector3 upVec =  Vector3(0.f, info->Size.y / 2.f, 0.f);
	Vector3 rightVec = Vector3(info->Size.x / 2.f, 0.f, 0.f);
	Vector3 downVec = Vector3(0.f, -info->Size.y / 2.f, 0.f);
	Vector3 leftVec = Vector3(-info->Size.x / 2.f, 0.f, 0.f);

	// ccw ����� ����, �������� ��ΰ� Ÿ���� ��� ��� �����ϴ��� �Ǻ��Ѵ�.
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
		// ������ ������
		else if (crossRight == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx - navData->GetPathFindTileCountX() + 1))
			{
				return true;
			}
		}

		float crossUpRight = crossUp * crossRight;

		// ��������� ���� Ÿ���� ������ ���� ���
		if (crossUpRight < 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx + 1))
			{
				return true;
			}
		}
		return false;
	}

	// ���� ��
	else if (moveDirection.x < 0 && moveDirection.y > 0)
	{
		float crossUp = moveDirection.Cross(upVec).z;
		float crossLeft = moveDirection.Cross(leftVec).z;

		// ���� �������� ���� ���
		if (crossUp == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx + navData->GetPathFindTileCountX() + 1))
			{
				return true;
			}
		}
		// ��
		else if (crossLeft == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx + navData->GetPathFindTileCountX() - 1))
			{
				return true;
			}
		}

		float crossLeftUp = crossLeft * crossUp;

		// ���� ��
		if (crossLeftUp < 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx + navData->GetPathFindTileCountX()))
			{
				return true;
			}
		}
		return false;
	}

	// ���� �Ʒ�
	else if (moveDirection.x < 0 && moveDirection.y < 0)
	{
		float crossDown = moveDirection.Cross(downVec).z;
		float crossLeft = moveDirection.Cross(leftVec).z;

		// �Ʒ�
		if (crossDown == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx - navData->GetPathFindTileCountX() - 1))
			{
				return true;
			}
		}
		// ��
		else if (crossLeft == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx + navData->GetPathFindTileCountX() - 1))
			{
				return true;
			}
		}

		float crossDownLeft = crossDown * crossLeft;

		// ���� �Ʒ�
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

		// ������ ������
		if (crossRight == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx - navData->GetPathFindTileCountX() + 1))
			{
				return true;
			}
		}
		// �Ʒ�
		else if (crossDown == 0)
		{
			if (!navData->IsReachableTile(mCurOnIdx - navData->GetPathFindTileCountX() - 1))
			{
				return true;
			}
		}

		float crossRightDown = crossRight * crossDown;
		// ������ �Ʒ�
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

	// �÷��̾� ������ �̹� �ٸ� AI�� �������� ���� Ÿ���� �ִ��� ã�Ƽ� ��ȯ�Ѵ�.
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

	// ������ ���� �������� ���� Ÿ���� ã�´�.
	int tryCount = 0;
	int searchRangeMin = -2;
	int searchRangeMax = 2;

	// 3�� �̻� ������ ��� �������� �����.
	while (tryCount < 3)
	{
		++tryCount;

		// �簢�� ���� �� �Ʒ� Ž��
		for (int x = searchRangeMin; x <= searchRangeMax; ++x)
		{
			int idxX = playerIdxX + x;
			int idxY = playerIdxY + searchRangeMax;
			
			// ���� �����ڸ� Ž��
			if (eTileType::Normal == tileMap->GetPathFindTile(idxX, idxY)->Type)
			{
				return (idxY * tileMap->GetPathFindTileCountX()) + idxX;
			}

			// �Ʒ��� �����ڸ� Ž��
			idxY = playerIdxY + searchRangeMin;
			if (eTileType::Normal == tileMap->GetPathFindTile(idxX, idxY)->Type)
			{
				return (idxY * tileMap->GetPathFindTileCountX()) + idxX;
			}
		}

		// �簢�� ���� ��, ������ Ž��
		for (int y = searchRangeMin + 1; y <= searchRangeMax - 1; ++y)
		{
			int idxX = playerIdxX + searchRangeMin;
			int idxY = playerIdxY + y;
			
			// ���� �����ڸ� Ž��
			if (eTileType::Normal == tileMap->GetPathFindTile(idxX, idxY)->Type)
			{
				return (idxY * tileMap->GetPathFindTileCountX()) + idxX;
			}

			// ������ �����ڸ� Ž��
			idxX = playerIdxX + searchRangeMax;
			if (eTileType::Normal == tileMap->GetPathFindTile(idxX, idxY)->Type)
			{
				return (idxY * tileMap->GetPathFindTileCountX()) + idxX;
			}
		}

		++searchRangeMax;
		--searchRangeMin;
	}

	// Ž�� ������ ���
	return -1;
}
