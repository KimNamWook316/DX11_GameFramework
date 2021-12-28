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
	// TODO : �浹ó��
}

CColliderComponent* CCollisionSection::DoCollideMouse(bool bIs2D, float deltaTime)
{
	if (bIs2D)
	{
		Vector2 mousePos = CInput::GetInst()->GetMouseWorldPos();

		size_t size = mVecCollider.size();
	
		if (size > 1)
		{
			// Y�� ����, �Ʒ��� �ִ� ������Ʈ���� �浹 ó�� �ϱ� ���� ����
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
