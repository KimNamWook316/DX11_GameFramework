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
	
	mMesh = nullptr;
	mCBuffer = nullptr;
}

CColliderComponent::CColliderComponent(const CColliderComponent& com) :
	CSceneComponent(com)
{
	mbMouseCollision = false;
	mbCheckCurrentSection = false;
	mbIsRender = com.mbIsRender;
	mProfile = com.mProfile;
	
	mCBuffer = com.mCBuffer->Clone();
}

CColliderComponent::~CColliderComponent()
{
	SAFE_DELETE(mCBuffer);

	auto iter = mPrevCollisionList.begin();
	auto iterEnd = mPrevCollisionList.end();

	// ���� ������ �浹 ����Ʈ ��ȸ�ϸ� �浹 ��� ���� �ݹ� ȣ��
	for (; iter != iterEnd; ++iter)
	{
		// �� �浹ü�� �浹�� ��ü���� �� ��ü �����϶�� �˸�
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

	// ��� ���� ����
	mCBuffer = new CColliderConstantBuffer;
	mCBuffer->Init();
	mCBuffer->SetColliderColor(Vector4(0.f, 1.f, 0.f, 1.f));

	return true;
}

void CColliderComponent::Start()
{
	CSceneComponent::Start();
}

void CColliderComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CColliderComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CColliderComponent::CheckCollision()
{
	// �ڽ��� �� �浹 �Ŵ����� ���
	mScene->GetCollision()->AddCollider(this);
	
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
}

void CColliderComponent::Load(FILE* fp)
{
	CSceneComponent::Load(fp);
	fread(&meColliderType, sizeof(eColliderType), 1, fp);
	fread(&mOffset, sizeof(Vector3), 1, fp);
	fread(&mMinPos, sizeof(Vector3), 1, fp);
	fread(&mMaxPos, sizeof(Vector3), 1, fp);
}

void CColliderComponent::CheckPrevCollisionSection()
{
	auto iter = mPrevCollisionList.begin();
	auto iterEnd = mPrevCollisionList.end();

	// ���� ������ �浹ü �ݺ���, ���� ������ �����ϴ��� Ȯ��
	// ��ġ�� ������ ���ٸ�, �浹 ���ɼ��� �����Ƿ� �浹�ߴ� ������ ������ �Ǵ��Ѵ�.
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
			(*iter)->CallCollisionCallBack(eCollisionState::Exit);

			// �浹��Ͽ��� ����
			DeletePrevCollision(*iter);
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
	auto iter = mCollisionCallBack[(int)state].begin();
	auto iterEnd = mCollisionCallBack[(int)state].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)(mResult);
	}
}

void CColliderComponent::CallCollisionMouseCallBack(eCollisionState state)
{
	auto iter = mCollisionMouseCallBack[(int)state].begin();
	auto iterEnd = mCollisionMouseCallBack[(int)state].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)(mMouseResult);
	}
}

void CColliderComponent::ClearFrame()
{
	mVecSectionIndex.clear();
	mCurrentCollisionList.clear();
	mbCheckCurrentSection = false;
}

void CColliderComponent::SetCollisionProfile(const std::string& name)
{
	mProfile = CCollisionManager::GetInst()->FindProfile(name);
}
