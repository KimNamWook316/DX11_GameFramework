#include "SceneComponent.h"

CSceneComponent::CSceneComponent()
{
	SetTypeID<CSceneComponent>();
	meComponentType = Component_Type::SCENE_COMP;
	mbIsRender = false;

	// ���ο� Transform ����
	mTransform = new CTransform;

	// �θ� ���� �� �ʱ�ȭ
	mTransform->mOwnerComponent = this;
	mTransform->Init();

	// �ٸ� SceneComponent�� �ڽ����� �� ���, AddChild�� ���� �θ� �����Ѵ�.
	// �� �����ڷ� ������ ���, �ֻ��� SceneComponent�� �� �ִ�.
	mParent = nullptr;
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)	:
	CComponent(com),
	mbIsRender(false)
{
	// ���ο� Transform Object���� ( Transform�� ������ �ʿ䰡 ���� )
	mTransform = com.mTransform->Clone();

	// Transform ��� ������ ����
	mTransform->mParentTransform = nullptr;
	mTransform->mVecChildTransform.clear();
	mTransform->mOwnerComponent = this;
	mParent = nullptr;

	size_t size = com.mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		// Clone() -> ���� ������Ʈ���� �ٽ� �� ������ ȣ�� 
		// -> ��� ���� ������Ʈ���� ���� ������
		// ���� ���� ������ ���鼭 �θ� ����
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
	// ����Ǹ鼭 ���� ������ ȣ��, CRef�� �ڽ��̱� ������ SharedPtr�� �����Ǹ�, ���۷��� ī��Ʈ üũ��
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

		// ���� child�� ������ Ž���Ѵ�. -> DFS Ž���� ���
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
	// �ڱ� �ڽ��� ���
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
