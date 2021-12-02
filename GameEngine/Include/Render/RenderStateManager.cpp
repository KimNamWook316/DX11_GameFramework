#include "RenderStateManager.h"
#include "BlendState.h"

CRenderStateManager::CRenderStateManager()
{
}

CRenderStateManager::~CRenderStateManager()
{
}

bool CRenderStateManager::Init()
{
	// Default Alpha Blending
	AddBlendInfo("AlphaBlend");
	// Multi-Sampling 사용하므로 두 번째 인자 true
	CreateBlendState("AlphaBlend", true, false);
	return false;
}

CRenderState* CRenderStateManager::FindRenderState(const std::string& name)
{
	auto iter = mMapRenderState.find(name);

	if (iter == mMapRenderState.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CRenderStateManager::SetBlendFactor(const std::string& name, const float r, const float g, const float b, const float a)
{
	CBlendState* state = (CBlendState*)FindRenderState(name);

	if (!state)
	{
		state = new CBlendState;
		state->SetName(name);
		mMapRenderState.insert(std::make_pair(name, state));
	}
	state->SetBlendFactor(r, g, b, a);
}

void CRenderStateManager::AddBlendInfo(const std::string& name, bool bIsBlendEnalbe, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND srcBlendAlpha, D3D11_BLEND destBlendAlpha, D3D11_BLEND_OP blendOpAlpha, UINT8 renderTargetWriteMask)
{
	CBlendState* state = (CBlendState*)FindRenderState(name);

	if (!state)
	{
		state = new CBlendState;
		state->SetName(name);
		mMapRenderState.insert(std::make_pair(name, state));
	}

	state->AddBlendInfo(bIsBlendEnalbe, srcBlend, destBlend, blendOp,
		srcBlendAlpha, destBlendAlpha, blendOpAlpha, renderTargetWriteMask);
}

bool CRenderStateManager::CreateBlendState(const std::string& name, bool bIsAlphaToCoverageEnable, bool bIsIndependentBlendEnable)
{
	CBlendState* state = (CBlendState*)FindRenderState(name);

	if (!state)
	{
		return false;
	}

	if (!state->CreateState(bIsAlphaToCoverageEnable, bIsIndependentBlendEnable))
	{
		SAFE_RELEASE(state);
		return false;
	}

	return true;
}
