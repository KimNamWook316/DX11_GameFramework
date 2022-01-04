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

	// �� ���ǿ� �浹ü�� �ϳ��ۿ� ���ٸ�, �浹 ������ �ʿ� ����
	if (count < 2)
	{
		return;
	}

	// �ε��� ����
	for (size_t i = 0; i < count - 1; ++i)
	{
		CColliderComponent* src = mVecCollider[i];

		for (size_t j = i + 1; j < count; ++j)
		{
			CColliderComponent* dest = mVecCollider[j];

			// �̹� ���� �浹�������� �浹�� �����ߴٸ�, �浹 ������ �����ִ� ���̹Ƿ� �� ���ǿ��� ������ �ʿ� ����
			if (src->IsExistInCurrentCollision(dest))
			{
				continue;
			}

			CollisionProfile* srcProfile = src->GetCollisionProfile();
			CollisionProfile* destProfile = dest->GetCollisionProfile();

			// �� �浹ü�� �����ִ� ä�ΰ��� �浹�� ��� �� ���õǴ� ���, �浹 ���� ���� �ʴ´�.
			if (srcProfile->vecInteraction[(int)destProfile->Channel] == eCollisionInteraction::Ignore &&
				destProfile->vecInteraction[(int)srcProfile->Channel] == eCollisionInteraction::Ignore)
			{
				continue;
			}

			// �浹 �����Ѵ�.
			if (src->DoCollide(dest))
			{
				// ���� �����ӿ� �浹�� ��Ͽ� ���ٸ�, ���� �� �浹�� ���۵� ��.
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

			// �浹�� ���� �ʾҴµ�, ���� �����ӿ� �浹�ߴٸ�, ���� �� �浹���¿��� ��� ��
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
			// 2D�� ��� Y-Sorting�ϹǷ�, Y���� ���� ��ü�� �տ� ǥ�õȴ�.
			// ���� Y���� ���� ���� ��ü�� �켱������ ��ŷ�Ǿ�� �ϹǷ�, Sort�Ѵ�.
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
