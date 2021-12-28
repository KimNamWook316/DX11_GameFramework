#pragma once

#include "../GameInfo.h"

class CCollisionSection
{
	friend class CSceneCollision;

private:
	CCollisionSection();
	~CCollisionSection();

public:
	void Init(const int indexX, const int indexY, const int indexZ, const int index,
		const Vector3& min, const Vector3& max,
		const Vector3& sectionSize,
		const Vector3& sectionTotalSize);
	void Clear();
	void AddCollider(class CColliderComponent* collider);
	void DoCollide(float deltaTime);
	CColliderComponent* DoCollideMouse(bool bIs2D, float deltaTime);

private:
	static int SortY(const void* src, const void* dest);

private:
	std::vector<class CColliderComponent*> mVecCollider;
	Vector3 mSectionSize;
	Vector3 mSectionTotalSize;
	Vector3 mMin;
	Vector3 mMax;
	int mIndex;
	int mIndexX;
	int mIndexY;
	int mIndexZ;
};

