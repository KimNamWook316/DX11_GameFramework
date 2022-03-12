#include "SceneCollision.h"
#include "../Engine.h"
#include "CollisionSection.h"
#include "../Component/ColliderComponent.h"
#include "../Input.h"
#include "Viewport.h"
#include "Scene.h"

CSceneCollision::CSceneCollision():
	mSectionInfo(nullptr),
	mScene(nullptr),
	mbWidgetClicked(false)
{
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
	// 마우스와 충돌한 오브젝트가 사라지면 마우스와 충돌한 충돌체 정보를 nullptr로
	// 오브젝트가 Die했을 때 처리
	auto iter = mColliderList.begin();
	auto iterEnd = mColliderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			// 마우스와 충돌한 오브젝트일경우 nullptr로 비어있게
			if (*iter == mMouseCollision)
			{
				mMouseCollision = nullptr;
			}
			iter = mColliderList.erase(iter);
			iterEnd = mColliderList.end();
			continue;
		}
		++iter;
	}

	// 충돌체를 분류해 각자의 영역으로 보낸다.
	classifyColliderBySection();

	// 현재 충돌 영역이 겹치는지 확인
	iter = mColliderList.begin();
	iterEnd = mColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetCheckCurrentSection())
		{
			continue;
		}
		(*iter)->CheckCurrentSection();

		// 이전 프레임에 충돌했다면, 충돌체들이 아직 같은 영역에 있는지 확인한다.
		(*iter)->CheckPrevCollisionSection();
	}

	// 마우스와 충돌처리
	DoCollideMouse(deltaTime);

	// 전체 Section 순회하며, 충돌처리
	size_t size = mSectionInfo->vecSection.size();
	for (size_t i = 0; i < size; ++i)
	{
		mSectionInfo->vecSection[i]->DoCollide(deltaTime);
		mSectionInfo->vecSection[i]->Clear();
	}

	iter = mColliderList.begin();
	iterEnd = mColliderList.end();
	
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->ClearFrame();
	}
}

void CSceneCollision::DoCollideMouse(float deltaTime)
{
	bool bCollideMouse = mbWidgetClicked;

	// UI와 마우스가 충돌하지 않은 경우, 물체와의 충돌
	if (!bCollideMouse)
	{
		// 마우스의 현재 충돌 영역 판별
		// 2D와 3D의 처리 다름, 3D의 경우 Picking기법 활용
		if (CEngine::GetInst()->GetEngineSpace() == eEngineSpace::Space2D)
		{
			Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();

			mousePos.x -= mSectionInfo->Min.x;
			mousePos.y -= mSectionInfo->Min.y;
			
			int idxX = 0;
			int idxY = 0;

			idxX = (int)(mousePos.x / mSectionInfo->SectionSize.x);
			idxY = (int)(mousePos.y / mSectionInfo->SectionSize.y);

			idxX = idxX < 0 ? -1 : idxX;
			idxY = idxY < 0 ? -1 : idxY;

			idxX = idxX > mSectionInfo->CountX -1 ? -1 : idxX;
			idxY = idxY > mSectionInfo->CountY -1 ? -1 : idxY;

			if (idxX != -1 && idxY != -1)
			{
				CColliderComponent* colliderMouse = mSectionInfo->vecSection[(size_t)idxY * (size_t)mSectionInfo->CountX + (size_t)idxX]->DoCollideMouse(true, deltaTime);

				if (colliderMouse)
				{
					bCollideMouse = true;

					if (colliderMouse != mMouseCollision)
					{
						colliderMouse->CallCollisionMouseCallBack(eCollisionState::Enter);
					}

					// 이전에 충돌한 물체와 다른 경우
					if (mMouseCollision && (colliderMouse != mMouseCollision))
					{
						mMouseCollision->CallCollisionMouseCallBack(eCollisionState::Exit);
					}

					mMouseCollision = colliderMouse;
				}
			}
		}
		else
		{
		}
	}
	// UI와 마우스가 충돌한 경우
	else
	{
		if (mMouseCollision)
		{
			// 이전에 마우스와 충돌했던 Collider는 이제 충돌에서 벗어난 상태
			mMouseCollision->CallCollisionMouseCallBack(eCollisionState::Exit);
			mMouseCollision = nullptr;
		}
	}

	if (!bCollideMouse)
	{
		// 마우스와 충돌하지 않았고, 이전 프레임에 충돌된 물체가 있었을 경우
		if (mMouseCollision)
		{
			mMouseCollision->CallCollisionMouseCallBack(eCollisionState::Exit);
			mMouseCollision = nullptr;
		}
	}
}

// CInput의 Update에서 호출
bool CSceneCollision::DoCollideWidget(float deltaTime)
{
	return mbWidgetClicked = mScene->GetViewport()->DoCollideMouse();
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
		mSectionInfo = new CollisionSectionInfo;
	}

	mSectionInfo->Center = Vector3(x,y,z);
	mSectionInfo->Min = mSectionInfo->Center - mSectionInfo->SectionTotalSize / 2.f;
	mSectionInfo->Max = mSectionInfo->Min + mSectionInfo->SectionSize;
}

void CSceneCollision::SetSectionMin(const Vector3& min)
{
	if (!mSectionInfo)
	{
		mSectionInfo = new CollisionSectionInfo;
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
		mSectionInfo = new CollisionSectionInfo;
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
		mSectionInfo = new CollisionSectionInfo;
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
		mSectionInfo = new CollisionSectionInfo;
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
	mColliderList.push_back(collider);
}

void CSceneCollision::DeleteCollider(CColliderComponent* collider)
{
	auto iter = mColliderList.begin();
	auto iterEnd = mColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == collider)
		{
			mColliderList.erase(iter);
			return;
		}
	}
}

void CSceneCollision::classifyColliderBySection()
{
	auto iter = mColliderList.begin();
	auto iterEnd = mColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
		{
			continue;
		}

		CColliderComponent* collider = *iter;

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
