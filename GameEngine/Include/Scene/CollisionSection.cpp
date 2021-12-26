#include "CollisionSection.h"

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
}

void CCollisionSection::AddCollider(CColliderComponent* collider)
{
	mVecCollider.push_back(collider);
}

void CCollisionSection::DoCollide(float deltaTime)
{
	// 角力 面倒 贸府
}
