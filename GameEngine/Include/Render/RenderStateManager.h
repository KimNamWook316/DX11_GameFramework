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

public:
    bool CreateDepthStencilState(const std::string& name, bool bDepthEnable = true,
        D3D11_DEPTH_WRITE_MASK depthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
        D3D11_COMPARISON_FUNC depthFunc = D3D11_COMPARISON_LESS,
        bool bStencilEnable = false,
        UINT8 stencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
        UINT8 stencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
        D3D11_DEPTH_STENCILOP_DESC frontFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
        D3D11_DEPTH_STENCILOP_DESC backFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS }
        );

private:
	std::unordered_map<std::string, CSharedPtr<class CRenderState>> mMapRenderState;
};

