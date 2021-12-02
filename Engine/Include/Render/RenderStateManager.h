#pragma once

#include "../GameInfo.h"

class CRenderStateManager
{
	friend class CRenderManager;

private:
	CRenderStateManager();
	~CRenderStateManager();

public:
	bool Init();

public:
	class CRenderState* FindRenderState(const std::string& name);

	void SetBlendFactor(const std::string& name, const float r, const float g,
		const float b, const float a);
	
	void AddBlendInfo(const std::string& name, 
		bool bIsBlendEnalbe = true, D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND srcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND destBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP blendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 renderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	
	bool CreateBlendState(const std::string& name,
		bool bIsAlphaToCoverageEnable, bool bIsIndependentBlendEnable);

private:
	std::unordered_map<std::string, CSharedPtr<class CRenderState>> mMapRenderState;
};

