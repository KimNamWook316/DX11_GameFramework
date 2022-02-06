#include "GameObject.h"
#include "../Scene/SceneManager.h"

CGameObject::CGameObject()	:
	mScene(nullptr),
	mParent(nullptr),
	mLifeSpan(0.f)
{
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject& obj)
{
	*this = obj;
	mRefCount = 0;

	if (obj.mRootSceneComponent)
	{
		mRootSceneComponent = obj.mRootSceneComponent->Clone();

		mRootSceneComponent->SetGameObject(this);

		mRootSceneComponent->SetThisToGameObject(this);
	}

	mVecObjectComponent.clear();

	size_t size = obj.mVecObjectComponent.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecObjectComponent.push_back(obj.mVecObjectComponent[i]->Clone());
		mVecObjectComponent[i]->SetGameObject(this);
	}
}

CGameObject::~CGameObject()
{
}

void CGameObject::Destroy()
{
	CRef::Destroy();
	
	mRootSceneComponent->Destroy();

	size_t size = mVecObjectComponent.size();
	for (size_t i = 0; i < size; ++i)
	{
		mVecObjectComponent[i]->Destroy();
	}
}

CComponent* CGameObject::FindComponent(const std::string& name)
{
	auto iter = mSceneComponentList.begin();
	auto iterEnd = mSceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}

	return nullptr;
}

void CGameObject::GetAllSceneComponentsName(std::vector<FindComponentName>& outNames)
{
	if (!mRootSceneComponent)
	{
		return;
	}

	mRootSceneComponent->GetAllSceneComponentsName(outNames);
}

void CGameObject::GetAllComponentsPointer(std::vector<CComponent*>& outPointers)
{
	if (mRootSceneComponent)
	{
		mRootSceneComponent->GetAllComponentsPointer(outPointers);
	}

	size_t size = mVecObjectComponent.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecObjectComponent[i]->GetAllComponentsPointer(outPointers);
	}
}

void CGameObject::SetScene(CScene* scene)
{
	mScene = scene;
}

void CGameObject::AddChildGameObject(CGameObject* obj)
{
	if (!obj)
	{
		return;
	}

	obj->mParent = this;
	mVecChildGameObj.push_back(obj);

	if (obj->GetRootSceneComponent())
	{
		mRootSceneComponent->AddChild(obj->GetRootSceneComponent());
	}
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Start()
{
	if (mRootSceneComponent)
	{
		mRootSceneComponent->Start();
	}

	size_t size = mVecObjectComponent.size();
	
	for (size_t i = 0; i < size; ++i)
	{
		mVecObjectComponent[i]->Start();
	}
}

void CGameObject::Update(float deltaTime)
{
	if (0.f < mLifeSpan)
	{
		mLifeSpan -= deltaTime;

		if (mLifeSpan <= 0.f)
		{
			Destroy();
			return;
		}
	}

	size_t size = mVecObjectComponent.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecObjectComponent[i]->Update(deltaTime);
	}

	if (mRootSceneComponent)
	{
		mRootSceneComponent->Update(deltaTime);
	}
}

void CGameObject::PostUpdate(float deltaTime)
{
	size_t size = mVecObjectComponent.size();

	for (size_t i = 0; i < size; ++i) 
	{
		mVecObjectComponent[i]->PostUpdate(deltaTime);
	}

	if (mRootSceneComponent)
	{
		mRootSceneComponent->PostUpdate(deltaTime);
	}
}

void CGameObject::PrevRender()
{
	size_t size = mVecObjectComponent.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecObjectComponent[i]->PrevRender();
	}

	if (mRootSceneComponent)
	{
		mRootSceneComponent->PrevRender();
	}
}

void CGameObject::AddCollision()
{
	if (mRootSceneComponent)
	{
		mRootSceneComponent->CheckCollision();
	}
}

void CGameObject::Render()
{
	size_t size = mVecObjectComponent.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecObjectComponent[i]->Render();
	}

	if (mRootSceneComponent)
	{
		mRootSceneComponent->Render();
	}
}

void CGameObject::PostRender()
{
	size_t size = mVecObjectComponent.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecObjectComponent[i]->PostRender();
	}

	if (mRootSceneComponent)
	{
		mRootSceneComponent->PostRender();
	}
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}

void CGameObject::Save(FILE* fp)
{
	CRef::Save(fp);

	if (mRootSceneComponent)
	{
		bool bRootExist = true;
		fwrite(&bRootExist, sizeof(bool), 1, fp);

		size_t typeID = mRootSceneComponent->GetTypeID();
		fwrite(&typeID, sizeof(size_t), 1, fp);

		mRootSceneComponent->Save(fp);
	}
	else
	{
		bool bRootExist = false;
		fwrite(&bRootExist, sizeof(bool), 1, fp);
	}

	int objComponentCount = (int)mVecObjectComponent.size();
	fwrite(&objComponentCount, sizeof(int), 1, fp);
	
	for (int i = 0; i < objComponentCount; ++i)
	{
		size_t typeID = mVecObjectComponent[i]->GetTypeID();
		fwrite(&typeID, sizeof(size_t), 1, fp);

		mVecObjectComponent[i]->Save(fp);
	}
}

void CGameObject::Load(FILE* fp)
{
	CRef::Load(fp);

	bool bRootExist = false;
	fread(&bRootExist, sizeof(bool), 1, fp);

	if (bRootExist)
	{
		size_t typeID = 0;
		fread(&typeID, sizeof(size_t), 1, fp);
		
		// CGameObject의 LoadComponent()를 통해 루트로 들어감
		CSceneManager::GetInst()->CallCreateComponentFunction(this, typeID);
		mRootSceneComponent->Load(fp);
	}

	int objComponentCount = 0;
	fread(&objComponentCount, sizeof(int), 1, fp);

	for (int i = 0; i < objComponentCount; ++i)
	{
		size_t typeID = 0;
		fread(&typeID, sizeof(size_t), 1, fp);

		// CGameObject의 LoadComponent()를 통해 벡터에 추가 
		CComponent* component = CSceneManager::GetInst()->CallCreateComponentFunction(this, typeID);
		component->Load(fp);
	}
 } 