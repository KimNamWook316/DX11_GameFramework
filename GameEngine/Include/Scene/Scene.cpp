#include "Scene.h"

CScene::CScene()
{
	mMode = new CSceneMode;
	mResource = new CSceneResource;

	mMode->mScene = this;
	mResource->mScene = this;
}

CScene::~CScene()
{
	SAFE_DELETE(mResource);
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
