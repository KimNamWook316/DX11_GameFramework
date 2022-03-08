#include "SceneComponent.h"
#include "../Render/RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"

CSceneComponent::CSceneComponent()
{
	SetTypeID<CSceneComponent>();
	meComponentType = eComponentType::SCENE_COMP;
	mbIsRender = false;

	// ���ο� Transform ����
	mTransform = new CTransform;

	// �θ� ���� �� �ʱ�ȭ
	mTransform->mOwnerComponent = this;
	mTransform->Init();

	// �ٸ� SceneComponent�� �ڽ����� �� ���, AddChild�� ���� �θ� �����Ѵ�.
	// �� �����ڷ� ������ ���, �ֻ��� SceneComponent�� �� �ִ�.
	mParent = nullptr;

	// �⺻ ���̾� ����
	mLayerName = "Default";
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)	:
	CComponent(com),
	mbIsRender(false)
{
	*this = com;

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

void CSceneComponent::Destroy()
{
	CComponent::Destroy();

	size_t size = mVecChild.size();
	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->Destroy();
	}
}

bool CSceneComponent::Init()
{
	return true;
}

void CSceneComponent::Start()
{
	CComponent::Start();

	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->Start();
	}
}

void CSceneComponent::Update(float deltaTime)
{
	mTransform->Update(deltaTime);

	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->Update(deltaTime);
	}
}

void CSceneComponent::PostUpdate(float deltaTime)
{
	mTransform->PostUpdate(deltaTime);

	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->PostUpdate(deltaTime);
	}
}

// �浹 ������Ʈ���� ������
void CSceneComponent::CheckCollision()
{
	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->CheckCollision();
	}
}

void CSceneComponent::PrevRender()
{
	if (mbIsRender)
	{
		// ���� ����̸� ���� ����Ʈ�� �ڽ� �߰�
		CRenderManager::GetInst()->AddRenderList(this);
	}

	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->PrevRender();
	}
}

void CSceneComponent::Render()
{
	mTransform->SetTransformBuffer();

	// �� ������Ʈ�� default ���� �ִϸ��̼� ����
	CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(false);
	CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();
}

void CSceneComponent::PostRender()
{
}

CSceneComponent* CSceneComponent::Clone()
{
	// ����Ǹ鼭 ���� ������ ȣ��, CRef�� �ڽ��̱� ������ SharedPtr�� �����Ǹ�, ���۷��� ī��Ʈ üũ��
	return new CSceneComponent(*this);
}

void CSceneComponent::Save(FILE* fp)
{
	CComponent::Save(fp);

	fwrite(&mbIsRender, sizeof(bool), 1, fp);

	int length = (int)mLayerName.length();
	fwrite(&length, sizeof(int), 1, fp);
	fwrite(mLayerName.c_str(), sizeof(char), length, fp);

	mTransform->Save(fp);

	int childCount = (int)mVecChild.size();
	fwrite(&childCount, sizeof(int), 1, fp);

	for (int i = 0; i < childCount; ++i)
	{
		size_t typeID = mVecChild[i]->GetTypeID();
		fwrite(&typeID, sizeof(size_t), 1, fp);

		mVecChild[i]->Save(fp);
	}
}

void CSceneComponent::Load(FILE* fp)
{
	CComponent::Load(fp);

	fread(&mbIsRender, sizeof(bool), 1, fp);
	
	char layerName[256] = {};
	int length = 0;
	fread(&length, sizeof(int), 1, fp);
	fread(layerName, sizeof(char), length, fp);
	mLayerName = layerName;

	mTransform->Load(fp);
	
	int childCount = 0;
	fread(&childCount, sizeof(int), 1, fp);

	for (int i = 0; i < childCount; ++i)
	{
		size_t typeID = 0;
		fread(&typeID, sizeof(size_t), 1, fp);
		
		CComponent* component = CSceneManager::GetInst()->CallCreateComponentFunction(mObject, typeID);
		component->Load(fp);
		static_cast<CSceneComponent*>(component)->mParent = this;
		mVecChild.push_back((CSceneComponent*)component);

		mTransform->mVecChildTransform.push_back(mVecChild[i]->mTransform);
		mVecChild[i]->mTransform->mParentTransform = mTransform;
	}
}

void CSceneComponent::GetAllSceneComponentsName(std::vector<FindComponentName>& outNames)
{
	FindComponentName name;

	name.Name = mName;

	if (mParent)
	{
		name.ParentName = mParent->GetName();
	}

	outNames.push_back(name);

	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->GetAllSceneComponentsName(outNames);
	}
}

void CSceneComponent::GetAllComponentsPointer(std::vector<CComponent*>& outVecPointers)
{
	outVecPointers.push_back((CComponent*)this);

	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->GetAllComponentsPointer(outVecPointers);
	}
}

void CSceneComponent::GetAllChildPointer(std::vector<CComponent*>& outVecPointers)
{
	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		outVecPointers.push_back(mVecChild[i]);
	}
}

// GameObject�� �� ������Ʈ ����Ʈ�� �� ��ü�� ����Ѵ�.
void CSceneComponent::SetThisToGameObject(CGameObject* obj)
{
	obj->AddSceneComponent(this);

	size_t size = mVecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecChild[i]->SetThisToGameObject(obj);
	}
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

// �� ������Ʈ�� ���� ������Ʈ�� ����Ѵ�.
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
