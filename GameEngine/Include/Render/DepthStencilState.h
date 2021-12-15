#pragma once

#include "RenderState.h"

class CDepthStencilState :
    public CRenderState
{
    friend class CRenderStateManager;

protected:
    CDepthStencilState();
    virtual ~CDepthStencilState();

public:
    bool CreateState(bool bDepthEnable = true,
        D3D11_DEPTH_WRITE_MASK depthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
        D3D11_COMPARISON_FUNC depthFunc = D3D11_COMPARISON_LESS,
        bool bStencilEnable = false,
        UINT8 stencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
        UINT8 stencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
        D3D11_DEPTH_STENCILOP_DESC frontFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
        D3D11_DEPTH_STENCILOP_DESC backFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS }
        );

public:
    virtual void SetState() override;
    virtual void ResetState() override;

protected:
    UINT mStencilRef;
    UINT mPrevStencilRef;
};

