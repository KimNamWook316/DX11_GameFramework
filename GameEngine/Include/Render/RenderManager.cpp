#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/SceneComponent.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "RenderStateManager.h"
#include "RenderState.h"
#include "../Engine.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	mRenderStateManager(nullptr),
	mStandard2DBuffer(nullptr),
	mDepthDisableState(nullptr),
	mAlphaBlendState(nullptr)
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
		// ������Ʈ�� ������ �ִ� ��������Ʈ�� �� �����Ŵ����� ���� ��
		if ((*iter)->Name == pComponent->GetLayerName())
		{
			// �� ���̾ �޾ƿ´�.
			layer = *iter;
			break;
		}
	}

	// �ش�Ǵ� ���̾ ������, �߰� �� ��
	if (!layer)
	{
		assert(false);
		return;
	}

	// �������� (����ȭ) -> ���� ����� ��Ÿ�ӿ��� �߰��Ǵ� ��찡 ������ �ſ� ����.
	// ���� �̸� ������ �Ҵ��س��� ���� �ӵ��鿡�� �������� ������?
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

	// Render priority�� ���� ����
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

	RenderLayer* layer = new RenderLayer;
	layer->Name = "Back";
	layer->LayerPriority = 0;
	mRenderLayerList.push_back(layer);

	// Defulat Layer
	layer = new RenderLayer;
	layer->Name = "Default";
	layer->LayerPriority = 1;
	mRenderLayerList.push_back(layer);

	// Particle Layer
	layer = new RenderLayer;
	layer->Name = "Particle";
	layer->LayerPriority = 2;
	mRenderLayerList.push_back(layer);

	// Widget Component Layer
	layer = new RenderLayer;
	layer->Name = "ScreenWidgetComponent";
	layer->LayerPriority = 3;
	mRenderLayerList.push_back(layer);

	// Depth Disable �� ����
	mDepthDisableState = mRenderStateManager->FindRenderState("DepthDisable");
	mAlphaBlendState = mRenderStateManager->FindRenderState("AlphaBlend");

	return true;
}

void CRenderManager::Render()
{
	// 2D������ Depth ������� �ʴ� State����
	if (CEngine::GetInst()->GetEngineSpace() == eEngineSpace::Space2D)
	{
	}

	{
		auto iter = mRenderLayerList.begin();
		auto iterEnd = mRenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderCount = 0;
		}
	}

	// ���� ����ִ� ������Ʈ ����Ʈ �����͸� ���鼭, ���� ����� ��� ��������Ʈ�� �߰�
	{
		auto iter = mpObjList->begin();
		auto iterEnd = mpObjList->end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->IsEnable())
			{
				(*iter)->PrevRender();
			}
		}
	}

	// ���� ����
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
	
	mDepthDisableState->SetState();

	// UI ����
	mAlphaBlendState->SetState();
	
	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	// ���콺 ����
	CWidgetWindow* mouseWidget = CEngine::GetInst()->GetMouseWidget();
	
	if (mouseWidget)
	{
		mouseWidget->Render();
	}
	mAlphaBlendState->ResetState();

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
