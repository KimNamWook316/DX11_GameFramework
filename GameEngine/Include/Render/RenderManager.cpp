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

	{
		auto iter = mpObjList->begin();
		auto iterEnd = mpObjList->end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->PrevRender();
		}
	}

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
