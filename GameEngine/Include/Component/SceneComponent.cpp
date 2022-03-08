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

	// 새로운 Transform 생성
	mTransform = new CTransform;

	// 부모 설정 및 초기화
	mTransform->mOwnerComponent = this;
	mTransform->Init();

	// 다른 SceneComponent의 자식으로 들어갈 경우, AddChild를 통해 부모를 설정한다.
	// 이 생성자로 생성될 경우, 최상위 SceneComponent일 수 있다.
	mParent = nullptr;

	// 기본 레이어 설정
	mLayerName = "Default";
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)	:
	CComponent(com),
	mbIsRender(false)
{
	*this = com;

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

// 충돌 컴포넌트에서 재정의
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
		// 렌더 대상이면 렌더 리스트에 자신 추가
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

	// 씬 컴포넌트의 default 값은 애니메이션 없음
	CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(false);
	CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();
}

void CSceneComponent::PostRender()
{
}

CSceneComponent* CSceneComponent::Clone()
{
	// 복사되면서 복사 생성자 호출, CRef의 자식이기 때문에 SharedPtr로 관리되며, 레퍼런스 카운트 체크함
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

// GameObject의 씬 컴포넌트 리스트에 이 객체를 등록한다.
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

// 이 컴포넌트의 주인 오브젝트를 등록한다.
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
