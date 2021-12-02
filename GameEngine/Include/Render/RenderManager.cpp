#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/SceneComponent.h"
#include "RenderStateManager.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	mRenderStateManager(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE(mRenderStateManager);
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

	mRenderStateManager = new CRenderStateManager;
	mRenderStateManager->Init();

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

CRenderState* CRenderManager::FindRenderState(const std::string& name)
{
	return mRenderStateManager->FindRenderState(name);
}

void CRenderManager::SetBlendFactor(const std::string& name, const float r, const float g, const float b, const float a)
{
	mRenderStateManager->SetBlendFactor(name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& name, bool bIsBlendEnalbe, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND srcBlendAlpha, D3D11_BLEND destBlendAlpha, D3D11_BLEND_OP blendOpAlpha, UINT8 renderTargetWriteMask)
{
	mRenderStateManager->AddBlendInfo(name, bIsBlendEnalbe, srcBlend, destBlend, blendOp, srcBlendAlpha, destBlendAlpha, blendOpAlpha, renderTargetWriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& name, bool bIsAlphaToCoverageEnable, bool bIsIndependentBlendEnable)
{
	return mRenderStateManager->CreateBlendState(name, bIsAlphaToCoverageEnable, bIsIndependentBlendEnable);
}
