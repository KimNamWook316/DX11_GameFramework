#include "GameObject.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Component/NavAgentComponent.h"
#include "../PathManager.h"

CGameObject::CGameObject()	:
	mScene(nullptr),
	mParent(nullptr),
	mLifeSpan(0.f),
	mbExcludeFromSave(false)
{
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject& obj)
{
	*this = obj;
	mRefCount = 0;

	if (obj.mRootSceneComponent)
	{
		mRootSceneComponent = nullptr;
		mSceneComponentList.clear();
		
		mRootSceneComponent = obj.mRootSceneComponent->Clone();

		mRootSceneComponent->SetScene(mScene);

		mRootSceneComponent->SetGameObject(this);

		mRootSceneComponent->SetThisToGameObject(this);
	}

	mVecObjectComponent.clear();

	size_t size = obj.mVecObjectComponent.size();

	for (size_t i = 0; i < size; ++i)
	{
		mVecObjectComponent.push_back(obj.mVecObjectComponent[i]->Clone());
		mVecObjectComponent[i]->SetScene(mScene);
		mVecObjectComponent[i]->SetGameObject(this);
	}

	mScene->AddObject(this);
}

CGameObject::~CGameObject()
{
}

void CGameObject::Enable(bool bEnable)
{
	mEnable = bEnable;

	if (mRootSceneComponent)
	{
		mRootSceneComponent->Enable(bEnable);
	}
	
	size_t size = mVecObjectComponent.size();
	for (size_t i = 0; i < size; ++i)
	{
		mVecObjectComponent[i]->Enable(bEnable);
	}
}

void CGameObject::Destroy()
{
	CRef::Destroy();
	
	if (mRootSceneComponent)
	{
		mRootSceneComponent->Destroy();
	}

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

	auto iterObj = mVecObjectComponent.begin();
	auto iterObjEnd = mVecObjectComponent.end();

	for (; iterObj != iterObjEnd; ++iterObj)
	{
		if (name == (*iterObj)->GetName())
		{
			return *iterObj;
		}
	}

	return nullptr;
}

void CGameObject::DeleteComponent(CComponent* comp)
{
	if (!comp)
	{
		return;
	}

	if (eComponentType::SCENE_COMP == comp->GetComponentType())
	{
		CSceneComponent* sceneComp = static_cast<CSceneComponent*>(comp);

		// 루트 컴포넌트일 경우 가장 앞에 있는 자식 오브젝트를 루트로 올린다.
		if (mRootSceneComponent == comp)
		{
			std::vector<CComponent*> childs;
			sceneComp->GetAllChildPointer(childs);
			mRootSceneComponent = (CSceneComponent*)childs[0];	
			static_cast<CSceneComponent*>(mRootSceneComponent)->mParent = nullptr;
			
			size_t size = childs.size();
			for (size_t i = 1; i < size; ++i)
			{
				mRootSceneComponent->AddChild((CSceneComponent*)childs[i]);
			}
		}
		// 루트 컴포넌트가 아닌 경우, 이 컴포넌트의 모든 자식들을 이 컴포넌트의 부모에 연결한다.
		else
		{
			CSceneComponent* parent = sceneComp->mParent;

			std::vector<CComponent*> childs;
			sceneComp->GetAllChildPointer(childs);

			size_t size = childs.size();
			for (size_t i = 0; i < size; ++i)
			{
				parent->AddChild((CSceneComponent*)childs[i]);
			}
			
			parent->DeleteChild((CSceneComponent*)comp);
		}

		// 삭제 
		mSceneComponentList.remove(sceneComp);
	}
	else
	{
		auto iter = mVecObjectComponent.begin();
		auto iterEnd = mVecObjectComponent.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter) == comp)
			{
				break;
			}
		}

		// 삭제
		mVecObjectComponent.erase(iter);
	}
}

void CGameObject::DeleteComponent(const std::string& name)
{
	DeleteComponent(FindComponent(name));
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

void CGameObject::GetAllComponentsName(std::vector<FindComponentName>& outNames)
{
	GetAllSceneComponentsName(outNames);

	size_t size = mVecObjectComponent.size();

	for (size_t i = 0; i < size; ++i)
	{
		FindComponentName name;
		name.Name = mVecObjectComponent[i]->GetName();
		outNames.push_back(name);
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

void CGameObject::Move(const Vector3& endPos)
{
	size_t size = mVecObjectComponent.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (mVecObjectComponent[i]->CheckType<CNavAgentComponent>())
		{
			((CNavAgentComponent*)mVecObjectComponent[i].Get())->Move(endPos);
			break;
		}
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

	bool bIsPlayer = false;
	
	if (this == mScene->GetPlayerObj())
	{
		bIsPlayer = true;
	}

	fwrite(&bIsPlayer, sizeof(bool), 1, fp);

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

void CGameObject::Save(const char* fileName, const std::string& pathName)
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (info)
	{
		strcpy_s(fullPath, info->PathMultibyte);
	}
	strcat_s(fullPath, fileName);

	SaveFullPath(fullPath);
}

void CGameObject::SaveFullPath(const char* fullPath)
{
	FILE* fp = nullptr;
	fopen_s(&fp, fullPath, "wb");

	if (!fp)
	{
		return;
	}

	Save(fp);
	fclose(fp);
}

void CGameObject::Load(FILE* fp)
{
	CRef::Load(fp);

	bool bIsPlayer = false;
	fread(&bIsPlayer, sizeof(bool), 1, fp);
	
	if (bIsPlayer)
	{
		mScene->SetPlayerObj(this);
	}

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

void CGameObject::Load(const char* fileName, const std::string& pathName)
{
	char fullPath[MAX_PATH] = {};
	
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
	{
		strcpy_s(fullPath, info->PathMultibyte);
	}
	strcat_s(fullPath, fileName);

	LoadFullPath(fullPath);
}

void CGameObject::LoadFullPath(const char* fullPath)
{
	FILE* fp = nullptr;
	fopen_s(&fp, fullPath, "rb");

	if (!fp)
	{
		return;
	}

	Load(fp);
	fclose(fp);
}
