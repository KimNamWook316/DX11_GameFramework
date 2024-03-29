#include "ColliderComponent.h"
#include "../Collision/CollisionManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"

CColliderComponent::CColliderComponent()
{
	SetTypeID<CColliderComponent>();
	meComponentType = eComponentType::SCENE_COMP;
	mbIsRender = true;

	mbCheckCurrentSection = false;
	mProfile = nullptr;
	mbMouseCollision = false;
	
	mCBuffer = nullptr;
}

CColliderComponent::CColliderComponent(const CColliderComponent& com) :
	CSceneComponent(com)
{
	meComponentType = eComponentType::SCENE_COMP;

	mbMouseCollision = false;
	mbCheckCurrentSection = false;
	mbIsRender = com.mbIsRender;
	mProfile = com.mProfile;
	mMesh = com.mMesh;
	mShader = com.mShader;
	
	mCBuffer = com.mCBuffer->Clone();
	mCBuffer->Init();
}

CColliderComponent::~CColliderComponent()
{
	SAFE_DELETE(mCBuffer);

	auto iter = mPrevCollisionList.begin();
	auto iterEnd = mPrevCollisionList.end();

	// 이전 프레임 충돌 리스트 순회하며 충돌 벗어날 때의 콜백 호출
	for (; iter != iterEnd; ++iter)
	{
		// 이 충돌체와 충돌한 객체에게 이 객체 삭제하라고 알림
		(*iter)->DeletePrevCollision(this);
		(*iter)->CallCollisionCallBack(eCollisionState::Exit);
		CallCollisionCallBack(eCollisionState::Exit);
	}
}

bool CColliderComponent::Init()
{
	if (!CSceneComponent::Init())
	{
		assert(false);
		return false;
	}

	// Defualt Profile : Object
	SetCollisionProfile("Object");

	// 상수 버퍼 생성
	mCBuffer = new CColliderConstantBuffer;
	mCBuffer->Init();
	mCBuffer->SetColliderColor(Vector4(0.f, 1.f, 0.f, 1.f));

	mShader = CResourceManager::GetInst()->FindShader("ColliderShader");

	return true;
}

void CColliderComponent::Start()
{
	CSceneComponent::Start();

	if (!mCBuffer)
	{
		mCBuffer = new CColliderConstantBuffer;
		mCBuffer->Init();
		mCBuffer->SetColliderColor(Vector4(0.f, 1.f, 0.f, 1.f));
	}
	if (!mShader)
	{
		mShader = CResourceManager::GetInst()->FindShader("ColliderShader");
	}

	// 자신을 충돌 매니저에 등록
	mScene->GetCollision()->AddCollider(this);
}

void CColliderComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CColliderComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

// TODO : 이 함수 필요한가?
void CColliderComponent::CheckCollision()
{
	CSceneComponent::CheckCollision();
}

void CColliderComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CColliderComponent::Render()
{
	CSceneComponent::Render();
}

void CColliderComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CColliderComponent* CColliderComponent::Clone()
{
	assert(false);
	return nullptr;
}

void CColliderComponent::Save(FILE* fp)
{
	CSceneComponent::Save(fp);
	fwrite(&meColliderType, sizeof(eColliderType), 1, fp);
	fwrite(&mOffset, sizeof(Vector3), 1, fp);
	fwrite(&mMinPos, sizeof(Vector3), 1, fp);
	fwrite(&mMaxPos, sizeof(Vector3), 1, fp);
	int length = mProfile->Name.length();
	fwrite(&length, sizeof(int), 1, fp);
	fwrite(mProfile->Name.c_str(), sizeof(char), length, fp);
}

void CColliderComponent::Load(FILE* fp)
{
	CSceneComponent::Load(fp);
	fread(&meColliderType, sizeof(eColliderType), 1, fp);
	fread(&mOffset, sizeof(Vector3), 1, fp);
	fread(&mMinPos, sizeof(Vector3), 1, fp);
	fread(&mMaxPos, sizeof(Vector3), 1, fp);
	
	char profileName[128] = {};
	int length = 0;
	fread(&length, sizeof(int), 1, fp);
	fread(profileName, sizeof(char), length, fp);
	mProfile = CCollisionManager::GetInst()->FindProfile(profileName);
}

void CColliderComponent::CheckPrevCollisionSection()
{
	auto iter = mPrevCollisionList.begin();
	auto iterEnd = mPrevCollisionList.end();

	// 이전 프레임 충돌체 반복해, 같은 영역에 존재하는지 확인
	// 겹치는 영역이 없다면, 충돌 가능성이 없으므로 충돌했다 떨어진 것으로 판단한다.
	for (; iter != iterEnd;)
	{
		bool bOverlap = false;
		size_t size = mVecSectionIndex.size();
		for (size_t i = 0; i < size; ++i)
		{
			size_t destSize = (*iter)->mVecSectionIndex.size();
			for (size_t j = 0; j < destSize; ++j)
			{
				if (mVecSectionIndex[i] == (*iter)->mVecSectionIndex[j])
				{
					bOverlap = true;
					break;
				}
			}
			if (bOverlap)
			{
				break;
			}
		}

		if (!bOverlap)
		{
			// CallBack
			CallCollisionCallBack(eCollisionState::Exit);

			if ((*iter)->IsEnable())
			{
				(*iter)->CallCollisionCallBack(eCollisionState::Exit);
			}

			// 충돌목록에서 삭제
			(*iter)->DeletePrevCollision(this);

			iter = mPrevCollisionList.erase(iter);
			iterEnd = mPrevCollisionList.end();
			continue;
		}
		++iter;
	}
}

void CColliderComponent::AddPrevCollision(CColliderComponent* collider)
{
	mPrevCollisionList.push_back(collider);
}

void CColliderComponent::DeletePrevCollision(CColliderComponent* collider)
{
	auto iter = mPrevCollisionList.begin();
	auto iterEnd = mPrevCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == collider)
		{
			mPrevCollisionList.erase(iter);
			return;
		}
	}
}

bool CColliderComponent::EmptyPrevCollision()
{
	return mPrevCollisionList.empty();
}

bool CColliderComponent::IsExistInPrevCollision(CColliderComponent* collider)
{
	auto iter = mPrevCollisionList.begin();
	auto iterEnd = mPrevCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == collider)
		{
			return true;
		}
	}
	return false;
}

bool CColliderComponent::IsExistInCurrentCollision(CColliderComponent* collider)
{
	auto iter = mCurrentCollisionList.begin();
	auto iterEnd = mCurrentCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == collider)
		{
			return true;
		}
	}
	return false;
}

void CColliderComponent::AddCurrentFrameCollision(CColliderComponent* collider)
{
	if (!IsExistInCurrentCollision(collider))
	{
		mCurrentCollisionList.push_back(collider);
	}
}

void CColliderComponent::CallCollisionCallBack(eCollisionState state)
{
	if (!mResult.Dest->IsEnable())
	{
		return;
	}

	if (mEnable)
	{
		auto iter = mCollisionCallBack[(int)state].begin();
		auto iterEnd = mCollisionCallBack[(int)state].end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter).CallBack(mResult);
		}
	}
}

void CColliderComponent::CallCollisionMouseCallBack(eCollisionState state)
{
	if (eCollisionState::Exit == state)
	{
		mbMouseCollision = false;
	}

	auto iter = mCollisionMouseCallBack[(int)state].begin();
	auto iterEnd = mCollisionMouseCallBack[(int)state].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter).CallBack(mMouseResult);
	}
}

void CColliderComponent::ClearFrame()
{
	mVecSectionIndex.clear();
	mCurrentCollisionList.clear();
	mbCheckCurrentSection = false;
}

void CColliderComponent::DeleteCollisionCallBack(void* obj, eCollisionState state)
{
	auto iter = mCollisionCallBack[(int)state].begin();
	auto iterEnd = mCollisionCallBack[(int)state].end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter).Obj == obj)
		{
			iter = mCollisionCallBack[(int)state].erase(iter);
			return;
		}
	}
}

void CColliderComponent::DeleteMouseCollisionCallBack(void* obj, eCollisionState state)
{
	auto iter = mCollisionMouseCallBack[(int)state].begin();
	auto iterEnd = mCollisionMouseCallBack[(int)state].end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter).Obj == obj)
		{
			iter = mCollisionMouseCallBack[(int)state].erase(iter);
			return;
		}
	}
}

void CColliderComponent::SetCollisionProfile(const std::string& name)
{
	mProfile = CCollisionManager::GetInst()->FindProfile(name);
}
