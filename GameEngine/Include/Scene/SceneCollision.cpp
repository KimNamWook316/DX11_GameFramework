#include "SceneCollision.h"
#include "../Engine.h"
#include "CollisionSection.h"
#include "../Component/ColliderComponent.h"

CSceneCollision::CSceneCollision():
	mSectionInfo(nullptr),
	mScene(nullptr)
{
	mVecCollider.reserve(10000);
}

CSceneCollision::~CSceneCollision()
{
	size_t size = mSectionInfo->vecSection.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mSectionInfo->vecSection[i]);
	}

	SAFE_DELETE(mSectionInfo);
}

bool CSceneCollision::Init()
{
	// Default
	SetSectionSize(1000.f, 1000.f, 1.f);
	SetSectionCenter(0.f, 0.f, 0.f);
	SetSectionCount(10, 10);
	CreateSection();

	return true;
}

void CSceneCollision::Start()
{
}

void CSceneCollision::DoCollide(float deltaTime)
{
	// 충돌체를 분류해 각자의 영역으로 보낸다.
	classifyColliderBySection();



	// 초기화
	mVecCollider.clear();
}

void CSceneCollision::SetSectionSize(const Vector3& size)
{
	if (!mSectionInfo)
	{
		return;
	}

	mSectionInfo->SectionSize = size;
	mSectionInfo->SectionTotalSize = mSectionInfo->SectionSize * Vector3((float)mSectionInfo->CountX, 
		(float)mSectionInfo->CountY, (float)mSectionInfo->CountZ);
}

void CSceneCollision::SetSectionSize(const float x, const float y, const float z)
{
	if (!mSectionInfo)
	{
		return;
	}

	mSectionInfo->SectionSize = Vector3(x,y,z);
	mSectionInfo->SectionTotalSize = mSectionInfo->SectionSize * Vector3((float)mSectionInfo->CountX, 
		(float)mSectionInfo->CountY, (float)mSectionInfo->CountZ);
}

void CSceneCollision::SetSectionCenter(const Vector3& center)
{
	if (!mSectionInfo)
	{
		return;
	}

	mSectionInfo->Center = center;
	mSectionInfo->Min = mSectionInfo->Center - mSectionInfo->SectionTotalSize / 2.f;
	mSectionInfo->Max = mSectionInfo->Min + mSectionInfo->SectionSize;
}

void CSceneCollision::SetSectionCenter(const float x, const float y, const float z)
{
	if (!mSectionInfo)
	{
		return;
	}

	mSectionInfo->Center = Vector3(x,y,z);
	mSectionInfo->Min = mSectionInfo->Center - mSectionInfo->SectionTotalSize / 2.f;
	mSectionInfo->Max = mSectionInfo->Min + mSectionInfo->SectionSize;
}

void CSceneCollision::SetSectionMin(const Vector3& min)
{
	if (!mSectionInfo)
	{
		return;
	}

	mSectionInfo->Min = min;
	mSectionInfo->SectionTotalSize = mSectionInfo->Max - mSectionInfo->Min;
	mSectionInfo->SectionSize = mSectionInfo->SectionTotalSize /
		Vector3((float)mSectionInfo->CountX, (float)mSectionInfo->CountY, (float)mSectionInfo->CountZ);
	mSectionInfo->Center = (mSectionInfo->Min + mSectionInfo->Max) / 2.f;
}

void CSceneCollision::SetSectionMin(const float x, const float y, const float z)
{
	if (!mSectionInfo)
	{
		return;
	}

	mSectionInfo->Min = Vector3(x,y,z);
	mSectionInfo->SectionTotalSize = mSectionInfo->Max - mSectionInfo->Min;
	mSectionInfo->SectionSize = mSectionInfo->SectionTotalSize /
		Vector3((float)mSectionInfo->CountX, (float)mSectionInfo->CountY, (float)mSectionInfo->CountZ);
	mSectionInfo->Center = (mSectionInfo->Min + mSectionInfo->Max) / 2.f;
}

void CSceneCollision::SetSectionMax(const Vector3& max)
{
	if (!mSectionInfo)
	{
		return;
	}

	mSectionInfo->Max = max;
	mSectionInfo->SectionTotalSize = mSectionInfo->Max - mSectionInfo->Min;
	mSectionInfo->SectionSize = mSectionInfo->SectionTotalSize /
		Vector3((float)mSectionInfo->CountX, (float)mSectionInfo->CountY, (float)mSectionInfo->CountZ);
	mSectionInfo->Center = (mSectionInfo->Min + mSectionInfo->Max) / 2.f;
}

void CSceneCollision::SetSectionMax(const float x, const float y, const float z)
{
	if (!mSectionInfo)
	{
		return;
	}

	mSectionInfo->Max = Vector3(x,y,z);
	mSectionInfo->SectionTotalSize = mSectionInfo->Max - mSectionInfo->Min;
	mSectionInfo->SectionSize = mSectionInfo->SectionTotalSize /
		Vector3((float)mSectionInfo->CountX, (float)mSectionInfo->CountY, (float)mSectionInfo->CountZ);
	mSectionInfo->Center = (mSectionInfo->Min + mSectionInfo->Max) / 2.f;
}

void CSceneCollision::SetSectionCount(const int countX, const int countY, const int countZ)
{
	mSectionInfo->CountX = countX;
	mSectionInfo->CountY = countY;
	mSectionInfo->CountZ = countZ;

	mSectionInfo->SectionTotalSize = mSectionInfo->SectionSize * Vector3((float)countX, (float)countY, (float)countZ);
	mSectionInfo->Min = mSectionInfo->Center - mSectionInfo->SectionTotalSize / 2.f;
	mSectionInfo->Max = mSectionInfo->Min + mSectionInfo->SectionTotalSize;
}

void CSceneCollision::CreateSection()
{
	if (!mSectionInfo)
	{
		mSectionInfo = new CollisionSectionInfo;
	}
	
	for (int z = 0; z < mSectionInfo->CountZ; ++z)
	{
		for (int y = 0; y < mSectionInfo->CountY; ++y)
		{
			for (int x = 0; x < mSectionInfo->CountX; ++x)
			{
				CCollisionSection* section = new CCollisionSection;
				
				section->Init(x, y, z, z * (mSectionInfo->CountX * mSectionInfo->CountZ) + y * (mSectionInfo->CountX) + x,
					mSectionInfo->Min, mSectionInfo->Max, mSectionInfo->SectionSize, mSectionInfo->SectionTotalSize);

				mSectionInfo->vecSection.push_back(section);
			}
		}
	}
}

void CSceneCollision::Clear()
{
	size_t size = mSectionInfo->vecSection.size();

	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(mSectionInfo->vecSection[i]);
	}
	SAFE_DELETE(mSectionInfo);
}

void CSceneCollision::AddCollider(CColliderComponent* collider)
{
	mVecCollider.push_back(collider);
}

void CSceneCollision::classifyColliderBySection()
{
	size_t size = mVecCollider.size();

	for (size_t i = 0; i < size; ++i)
	{
		CColliderComponent* collider = mVecCollider[i];

		Vector3 min = collider->GetMinPos();
		Vector3 max = collider->GetMaxPos();

		// 0, 0 기준으로 정렬
		min -= mSectionInfo->Min;
		max -= mSectionInfo->Min;

		int idxMinX, idxMinY, idxMinZ;
		int idxMaxX, idxMaxY, idxMaxZ;

		// 좌표로 인덱스 산출
		idxMinX = (int)(min.x / mSectionInfo->SectionSize.x);
		idxMinY = (int)(min.y / mSectionInfo->SectionSize.y);
		idxMinZ = (int)(min.z / mSectionInfo->SectionSize.z);

		idxMaxX = (int)(max.x / mSectionInfo->SectionSize.x);
		idxMaxY = (int)(max.y / mSectionInfo->SectionSize.y);
		idxMaxZ = (int)(max.z / mSectionInfo->SectionSize.z);

		// 예외처리
		idxMinX = idxMinX < 0 ? 0 : idxMinX;
		idxMinY = idxMinY < 0 ? 0 : idxMinY;
		idxMinZ = idxMinZ < 0 ? 0 : idxMinZ;
		
		idxMaxX = idxMaxX > mSectionInfo->CountX - 1 ? mSectionInfo->CountX - 1 : idxMaxX;
		idxMaxY = idxMaxY > mSectionInfo->CountY - 1 ? mSectionInfo->CountY - 1 : idxMaxY;
		idxMaxZ = idxMaxZ > mSectionInfo->CountZ - 1 ? mSectionInfo->CountZ - 1 : idxMaxZ;

		// 여러 섹션에 걸쳐있는 경우 한 번 이상 루프 돌게 된다.
		for (int z = idxMinZ; z <= idxMaxZ; ++z)
		{
			for (int y = idxMinY; y <= idxMaxY; ++y)
			{
				for (int x = idxMinX; x <= idxMaxX; ++x)
				{
					// 어떤 섹션 소속인지 최종 인덱스 산출
					int idx = z * (mSectionInfo->CountX * mSectionInfo->CountY) +
						y * (mSectionInfo->CountX) + x;

					mSectionInfo->vecSection[idx]->AddCollider(collider);
				}
			}
		}
	}
}
