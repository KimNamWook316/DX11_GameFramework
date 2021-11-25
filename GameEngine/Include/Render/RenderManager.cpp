#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/SceneComponent.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()
{
}

CRenderManager::~CRenderManager()
{
}

void CRenderManager::AddRenderList(CSceneComponent* pComponent)
{
	if (mRenderCount == (int)mVecRender.size())
	{
		mVecRender.resize((size_t)mRenderCount * 2);
	}

	mVecRender[mRenderCount] = pComponent;
	++mRenderCount;
}

bool CRenderManager::Init()
{
	mVecRender.resize(500);
	mRenderCount = 0;
	return true;
}

void CRenderManager::Render()
{
	mRenderCount = 0;

	// 씬이 들고있는 오브젝트 리스트 포인터를 돌면서, 렌더 대상일 경우 렌더리스트에 추가
	{
		auto iter = mpObjList->begin();
		auto iterEnd = mpObjList->end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->PrevRender();
		}
	}

	// 실제 렌더
	{
		for (int i = 0; i < mRenderCount; ++i)
		{
			mVecRender[i]->Render();
		}
	}

	{
		for (int i = 0; i < mRenderCount; ++i)
		{
			mVecRender[i]->PostRender();
		}
	}
}
