#include "CollisionSection.h"
#include "../Component/ColliderComponent.h"
#include "../Input.h"

CCollisionSection::CCollisionSection()	:
	mIndex(0),
	mIndexX(0),
	mIndexY(0),
	mIndexZ(0)
{
}

CCollisionSection::~CCollisionSection()
{
}

void CCollisionSection::Init(const int indexX, const int indexY, const int indexZ, const int index, const Vector3& min, const Vector3& max, const Vector3& sectionSize, const Vector3& sectionTotalSize)
{
	mIndexX = indexX;
	mIndexY = indexY;
	mIndexZ = indexZ;
	mIndex = index;
	mMin = min;
	mMax = max;
	mSectionSize = sectionSize;
	mSectionTotalSize = sectionTotalSize;

	mVecCollider.reserve(100);
}

void CCollisionSection::Clear()
{
	mVecCollider.clear();
}

void CCollisionSection::AddCollider(CColliderComponent* collider)
{
	mVecCollider.push_back(collider);
	collider->AddSectionIndex(mIndex);
}

void CCollisionSection::DoCollide(float deltaTime)
{
	size_t count = mVecCollider.size();

	// 이 섹션에 충돌체가 하나밖에 없다면, 충돌 수행할 필요 없음
	if (count < 2)
	{
		return;
	}

	// 인덱스 주의
	for (size_t i = 0; i < count - 1; ++i)
	{
		CColliderComponent* src = mVecCollider[i];

		for (size_t j = i + 1; j < count; ++j)
		{
			CColliderComponent* dest = mVecCollider[j];

			// 이미 이전 충돌영역에서 충돌을 수행했다면, 충돌 영역이 걸쳐있는 것이므로 이 섹션에서 수행할 필요 없음
			if (src->IsExistInCurrentCollision(dest))
			{
				continue;
			}

			CollisionProfile* srcProfile = src->GetCollisionProfile();
			CollisionProfile* destProfile = dest->GetCollisionProfile();

			// 두 충돌체가 속해있는 채널간의 충돌이 모두 다 무시되는 경우, 충돌 수행 하지 않는다.
			if (srcProfile->vecInteraction[(int)destProfile->Channel] == eCollisionInteraction::Ignore &&
				destProfile->vecInteraction[(int)srcProfile->Channel] == eCollisionInteraction::Ignore)
			{
				continue;
			}

			// 충돌 수행한다.
			if (src->DoCollide(dest))
			{
				// 이전 프레임에 충돌된 목록에 없다면, 지금 막 충돌이 시작된 것.
				if (!src->IsExistInPrevCollision(dest))
				{
					src->AddPrevCollision(dest);
					dest->AddPrevCollision(src);

					src->CallCollisionCallBack(eCollisionState::Enter);
					dest->CallCollisionCallBack(eCollisionState::Enter);
				}
				
				src->AddCurrentFrameCollision(dest);
				dest->AddCurrentFrameCollision(src);
			}

			// 충돌이 되지 않았는데, 이전 프레임에 충돌했다면, 지금 막 충돌상태에서 벗어난 것
			else if (src->IsExistInPrevCollision(dest))
			{
				src->DeletePrevCollision(dest);
				dest->DeletePrevCollision(src);

				src->CallCollisionCallBack(eCollisionState::Exit);
				dest->CallCollisionCallBack(eCollisionState::Exit);
			}
		}
	}
}

CColliderComponent* CCollisionSection::DoCollideMouse(bool bIs2D, float deltaTime)
{
	if (bIs2D)
	{
		Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();

		size_t size = mVecCollider.size();
	
		if (size > 1)
		{
			// 2D의 경우 Y-Sorting하므로, Y값이 낮은 물체가 앞에 표시된다.
			// 따라서 Y값이 가장 낮은 물체가 우선적으로 피킹되어야 하므로, Sort한다.
			qsort(&mVecCollider[0], size, sizeof(CColliderComponent*), CCollisionSection::SortY);
		}

		for(size_t i = 0; i < size; ++i)
		{
			if (mVecCollider[i]->DoCollideMouse(mousePos))
			{
				return mVecCollider[i];
			}
		}
	}
	else
	{

	}
	return nullptr;
}

int CCollisionSection::SortY(const void* src, const void* dest)
{
	CColliderComponent* srcCollider = *(CColliderComponent**)(src);
	CColliderComponent* destCollider = *(CColliderComponent**)(dest);
	
	if(srcCollider->GetMinPos().y < destCollider->GetMinPos().y)
	{
		return -1;
	}
	else if (srcCollider->GetMinPos().y > destCollider->GetMinPos().y)
	{
		return 1;
	}
	return 0;
}
