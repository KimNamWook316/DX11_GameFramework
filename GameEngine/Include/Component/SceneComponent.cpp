#include "SceneComponent.h"

CSceneComponent::CSceneComponent()
{
	SetTypeID<CSceneComponent>();
	meComponentType = Component_Type::SCENE_COMP;
	mbIsRender = false;

	// 새로운 Transform 생성
	mTransform = new CTransform;

	// 부모 설정 및 초기화
	mTransform->mOwnerComponent = this;
	mTransform->Init();

	// 다른 SceneComponent의 자식으로 들어갈 경우, AddChild를 통해 부모를 설정한다.
	// 이 생성자로 생성될 경우, 최상위 SceneComponent일 수 있다.
	mParent = nullptr;
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)	:
	CComponent(com),
	mbIsRender(false)
{
	// 새로운 Transform Object생성 ( Transform은 공유될 필요가 없음 )
	mTransform = com.mTransform->Clone();

	// Transform 멤버 변수들 설정
	mTransform->mParentTransform = nullptr;
	mTransform->mVecChildTransform.clear();
	mTransform->mOwnerComponent = this;
	mParent = nullptr;

	size_t size = com.mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		// Clone() -> 하위 오브젝트에서 다시 이 생성자 호출 
		// -> 모든 하위 오브젝트들이 새로 생성됨
		// 계층 구조 끝까지 돌면서 부모 설정
		CSceneComponent* cloneComponent = com.mVecChild[i]->Clone();

		cloneComponent->mParent = this;
		mVecChild.push_back(cloneComponent);

		cloneComponent->mTransform->mParentTransform = mTransform;
		mTransform->mVecChildTransform.push_back(cloneComponent->mTransform);
	}

}

CSceneComponent::~CSceneComponent()
{
	SAFE_DELETE(mTransform);
}

bool CSceneComponent::Init()
{
	return true;
}

void CSceneComponent::Update(float DeltaTime)
{
}

void CSceneComponent::PostUpdate(float DeltaTime)
{
}

void CSceneComponent::PrevRender()
{
}

void CSceneComponent::Render()
{
}

void CSceneComponent::PostRender()
{
}

CSceneComponent* CSceneComponent::Clone()
{
	// 복사되면서 복사 생성자 호출, CRef의 자식이기 때문에 SharedPtr로 관리되며, 레퍼런스 카운트 체크함
	return new CSceneComponent(*this);
}

void CSceneComponent::SetScene(CScene* scene)
{
	CComponent::SetScene(scene);

	mTransform->mScene = scene;

	size_t size = mVecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->SetScene(scene);
	}
}

void CSceneComponent::SetGameObject(CGameObject* obj)
{
	CComponent::SetGameObject(obj);

	mTransform->mObject = obj;

	size_t size = mVecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->SetGameObject(obj);
	}
}

void CSceneComponent::AddChild(CSceneComponent* child)
{
	child->mParent =this;

	mVecChild.push_back(child);

	child->mTransform->mParentTransform = mTransform;

	mTransform->mVecChildTransform.push_back(child->mTransform);
}

bool CSceneComponent::DeleteChild(CSceneComponent* deleteTarget)
{
	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (mVecChild[i] == deleteTarget)
		{
			auto iter = mVecChild.begin() + i;

			mVecChild.erase(iter);

			auto iterTrans = mTransform->mVecChildTransform.begin() + i;

			mTransform->mVecChildTransform.erase(iterTrans);
			return true;
		}

		// 현재 child의 끝까지 탐색한다. -> DFS 탐색과 비슷
		if (mVecChild[i]->DeleteChild(deleteTarget))
		{
			return true;
		}
	}
	return false;

}

bool CSceneComponent::DeleteChild(std::string& deleteTargetName)
{
	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (mVecChild[i]->GetName() == deleteTargetName)
		{
			auto iter = mVecChild.begin() + i;
			mVecChild.erase(iter);
			auto iterTrans = mTransform->mVecChildTransform.begin() + i;
			mTransform->mVecChildTransform.erase(iterTrans);
		}

		if (mVecChild[i]->DeleteChild(deleteTargetName))
		{
			return true;
		}
	}
	return false;
}

CSceneComponent* CSceneComponent::FindComponent(const std::string& name)
{
	// 자기 자신일 경우
	if (name == mName)
	{
		return this;
	}

	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		CSceneComponent* find = mVecChild[i]->FindComponent(name);

		if (find)
		{
			return find;
		}
	}

	return nullptr;
}
