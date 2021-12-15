#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/SceneComponent.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "RenderStateManager.h"
#include "RenderState.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	mRenderStateManager(nullptr),
	mStandard2DBuffer(nullptr),
	mDepthDisableState(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	auto iter = mRenderLayerList.begin();
	auto iterEnd = mRenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}
	mRenderLayerList.clear();

	SAFE_DELETE(mStandard2DBuffer);
	SAFE_DELETE(mRenderStateManager);
}

void CRenderManager::AddRenderList(CSceneComponent* pComponent)
{
	RenderLayer* layer = nullptr;

	auto iter = mRenderLayerList.begin();
	auto iterEnd = mRenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		// 컴포넌트가 가지고 있는 렌더리스트가 이 렌더매니저에 있을 때
		if ((*iter)->Name == pComponent->GetLayerName())
		{
			// 그 레이어를 받아온다.
			layer = *iter;
			break;
		}
	}

	// 해당되는 레이어가 없으면, 추가 안 함
	if (!layer)
	{
		assert(false);
		return;
	}

	// 리사이즈
	if (layer->RenderCount == (int)layer->RenderList.size())
	{
		layer->RenderList.resize((size_t)layer->RenderCount * 2);
	}

	layer->RenderList[layer->RenderCount] = pComponent;
	++layer->RenderCount;
}

void CRenderManager::CreateLayer(const std::string& name, const int priority)
{
	RenderLayer* layer = new RenderLayer;
	layer->Name = name;
	layer->LayerPriority = priority;

	mRenderLayerList.push_back(layer);

	// Render priority에 맞춰 정렬
	sort(mRenderLayerList.begin(), mRenderLayerList.end(), CRenderManager::sortLayer);
}

void CRenderManager::SetLayerPriority(const std::string& name, const int priority)
{
	auto iter = mRenderLayerList.begin();
	auto iterEnd = mRenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == name)
		{
			(*iter)->LayerPriority = priority;
			break;
		}
	}
	
	sort(mRenderLayerList.begin(), mRenderLayerList.end(), CRenderManager::sortLayer);
}

bool CRenderManager::Init()
{
	mRenderStateManager = new CRenderStateManager;
	mRenderStateManager->Init();

	mStandard2DBuffer = new CStandard2DConstantBuffer;
	mStandard2DBuffer->Init();

	// Defulat Layer
	RenderLayer* layer = new RenderLayer;
	layer->Name = "Default";
	layer->LayerPriority = 0;
	mRenderLayerList.push_back(layer);

	// TODO : Depth Disable 된 상태임
	mDepthDisableState = mRenderStateManager->FindRenderState("DepthDisable");

	return true;
}

void CRenderManager::Render()
{
	// TODO : 3D에서는 적용하지 않아야 함
	mDepthDisableState->SetState();

	{
		auto iter = mRenderLayerList.begin();
		auto iterEnd = mRenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderCount = 0;
		}
	}

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
		auto iter = mRenderLayerList.begin();
		auto iterEnd = mRenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; ++i)
			{
				(*iter)->RenderList[i]->Render();
			}
		}
	}

	{
		auto iter = mRenderLayerList.begin();
		auto iterEnd = mRenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; ++i)
			{
				(*iter)->RenderList[i]->PostRender();
			}
		}
	}

	// TODO : 3D에서는 적용하지 않아야 함
	mDepthDisableState->ResetState();
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

bool CRenderManager::sortLayer(RenderLayer* src, RenderLayer* dest)
{
	return src->LayerPriority < dest->LayerPriority;
}
