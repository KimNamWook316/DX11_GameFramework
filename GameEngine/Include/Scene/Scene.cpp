#include "Scene.h"
#include "SceneManager.h"
#include "../PathManager.h"

CScene::CScene()
{
	mMode = new CSceneMode;
	mResource = new CSceneResource;
	mCollision = new CSceneCollision;

	mMode->mScene = this;
	mResource->mScene = this;

	mbIsStart = false;

	mCollision->Init();
}

CScene::~CScene()
{
	SAFE_DELETE(mCollision);
	SAFE_DELETE(mResource);
}

void CScene::Start()
{
	mMode->Start();

	auto iter = mObjList.begin();
	auto iterEnd = mObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	mbIsStart = true;

	mCollision->Start();
}

void CScene::Update(float deltaTime)
{
	mMode->Update(deltaTime);

	auto iter = mObjList.begin();
	auto iterEnd = mObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			// Active상태가 아니면 리스트에서 삭제
			iter = mObjList.erase(iter);
			iterEnd = mObjList.end();
		}

		if (!(*iter)->IsEnable())
		{
			// 비활성화 상태라면 update하지 않음
			++iter;
			continue;
		}

		(*iter)->Update(deltaTime);
		++iter;
	}
}

void CScene::PostUpdate(float deltaTime)
{
	mMode->PostUpdate(deltaTime);

	auto iter = mObjList.begin();
	auto iterEnd = mObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = mObjList.erase(iter);
			iterEnd = mObjList.end();
		}

		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(deltaTime);
		++iter;
	}
}

void CScene::Save(const char* fileName, const std::string& pathName)
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

bool CScene::SaveFullPath(const char* fullPath)
{
	FILE* fp = nullptr;
	
	fopen_s(&fp, fullPath, "wb");

	if (!fp)
	{
		assert(false);
		return false;
	}

	size_t sceneModeType = mMode->GetTypeID();
	fwrite(&sceneModeType, sizeof(size_t), 1, fp);

	size_t objCount = mObjList.size();
	fwrite(&objCount, sizeof(size_t), 1, fp);

	auto iter = mObjList.begin();
	auto iterEnd = mObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		size_t objType = (*iter)->GetTypeID();
		fwrite(&objType, sizeof(size_t), 1, fp);

		(*iter)->Save(fp);
	}

	fclose(fp);
	return true;
}

void CScene::Load(const char* fileName, const std::string& pathName)
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};
	if (info)
	{
		strcpy_s(fullPath, info->PathMultibyte);
	}

	strcat_s(fullPath, fileName);

	LoadFullPath(fullPath);
}

bool CScene::LoadFullPath(const char* fullPath)
{
	FILE* fp = nullptr;
	
	fopen_s(&fp, fullPath, "rb");

	if (!fp)
	{
		assert(false);
		return false;
	}

	mObjList.clear();

	size_t sceneModeType = 0;
	fread(&sceneModeType, sizeof(size_t), 1, fp);

	// SceneMode 생성
	CSceneManager::GetInst()->CallCreateSceneModeFunction(this, sceneModeType);

	size_t objCount = 0;
	fread(&objCount, sizeof(size_t), 1, fp);

	for (size_t i = 0; i < objCount; ++i)
	{
		size_t objTypeID = 0;
		fread(&objTypeID, sizeof(size_t), 1, fp);

		CGameObject* obj = CSceneManager::GetInst()->CallCreateObjectFunction(this, objTypeID);

		obj->Load(fp);
	}

	fclose(fp);
	return true;
}
