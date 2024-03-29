#include "Scene.h"
#include "SceneManager.h"
#include "../PathManager.h"

CScene::CScene()
{
	mMode = new CSceneMode;
	mResource = new CSceneResource;
	mCollision = new CSceneCollision;
	mCameraManager = new CCameraManager;
	mNavManager = new CNavigationManager;
	mViewport = new CViewport;

	mMode->mScene = this;
	mResource->mScene = this;
	mCameraManager->mScene = this;
	mNavManager->mScene = this;
	mCollision->mScene = this;
	mViewport->mScene = this;

	mbIsStart = false;

	mCollision->Init();
	mCameraManager->Init();
	mNavManager->Init();
	mViewport->Init();

	mbBeChange = true;
}

CScene::~CScene()
{
	SAFE_DELETE(mCameraManager);
	SAFE_DELETE(mNavManager);
	SAFE_DELETE(mCollision);
	SAFE_DELETE(mResource);
	SAFE_DELETE(mViewport);
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
	mCameraManager->Start();
	mNavManager->Start();
	mViewport->Start();

	// 플레이어가 있고, 플레이어가 카메라를 가지고 있다면 플레이어 카메라를 기본 카메라로 설정
	CGameObject* playerObj = mMode->GetPlayerObj();
	if (playerObj)
	{
		CCameraComponent* playerCam = playerObj->FindSceneComponentFromType<CCameraComponent>();
		if (playerCam)
		{
			mCameraManager->SetCurrentCamera(playerCam);
		}
	}
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

			callCreatCallBack();
			continue;
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

	mCameraManager->Update(deltaTime);

	mNavManager->Update(deltaTime);

	// UI Viewport update
	mViewport->Update(deltaTime);
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
			callCreatCallBack();
			continue;
		}

		if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(deltaTime);
		++iter;
	}

	mCameraManager->PostUpdate(deltaTime);

	mViewport->PostUpdate(deltaTime);

	// 모든 PostUpdate 처리가 끝나고, World Position이 결정된 후에 충돌을 수행한다.
 
	// 포함된 충돌체들을 이용해 충돌처리 수행
	mCollision->DoCollide(deltaTime);
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
	size_t excludeCount = 0;

	// 저장에서 제외되어야 하는 오브젝트들은 제외한다.
	auto iter = mObjList.begin();
	auto iterEnd = mObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->IsExcludeFromSave())
		{
			++excludeCount;
		}
	}
	objCount -= excludeCount;

	fwrite(&objCount, sizeof(size_t), 1, fp);

	iter = mObjList.begin();
	iterEnd = mObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsExcludeFromSave())
		{
			size_t objType = (*iter)->GetTypeID();
			fwrite(&objType, sizeof(size_t), 1, fp);

			(*iter)->Save(fp);
		}
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

	auto iter = mObjList.begin();
	auto iterEnd = mObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	fclose(fp);
	return true;
}

bool CScene::SaveGameObject(const std::string& objName, const char* fileName, const std::string& pathName)
{	
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};
	if (info)
	{
		strcpy_s(fullPath, info->PathMultibyte);
	}

	strcat_s(fullPath, fileName);

	return SaveGameObjectFullPath(objName, fullPath);
}

bool CScene::SaveGameObjectFullPath(const std::string& objName, const char* fullPath)
{
	CGameObject* saveObj = FindObject(objName);

	if (!saveObj)
	{
		return false;
	}

	saveObj->SaveFullPath(fullPath);
}

CGameObject* CScene::LoadGameObject(std::string& outName, const char* fileName, const std::string& pathName)
{
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};
	if (info)
	{
		strcpy_s(fullPath, info->PathMultibyte);
	}

	strcat_s(fullPath, fileName);

	return LoadGameObjectFullPath(outName, fullPath);
}

CGameObject* CScene::LoadGameObjectFullPath(std::string& outName, const char* fullPath)
{
	CGameObject* obj = CreateGameObject<CGameObject>("temp");
	obj->LoadFullPath(fullPath);
	outName = obj->GetName();
	if (mbIsStart)
	{
		obj->Start();
	}

	callCreatCallBack();

	return obj;
}

void CScene::callCreatCallBack()
{
	auto iter = mCreateObjectCallBackList.begin();
	auto iterEnd = mCreateObjectCallBackList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)();
	}
}
