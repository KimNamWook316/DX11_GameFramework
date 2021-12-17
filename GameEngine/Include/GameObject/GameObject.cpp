#include "GameObject.h"

CGameObject::CGameObject()	:
	mScene(nullptr),
	mParent(nullptr)
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
