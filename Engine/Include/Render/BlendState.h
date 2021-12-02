#pragma once

#include "RenderState.h"

// 블렌딩은 추가 연산을 하기 때문에, 필요 없을 때는 하지 않아야 한다.
class CBlendState :
    public CRenderState
{
    friend class CRenderStateManager;

protected:
    CBlendState();
    virtual ~CBlendState();

public:
    virtual void SetState() override;
    virtual void ResetState() override;

public:
    void AddBlendInfo(bool bIsBlendEnalbe = true, D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA,
        D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD,
        D3D11_BLEND srcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND destBlendAlpha = D3D11_BLEND_ZERO,
        D3D11_BLEND_OP blendOpAlpha = D3D11_BLEND_OP_ADD,
        UINT8 renderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);

    bool CreateState(bool bIsAlphaToCoverageEnable, bool bIsIndependentBlendEnable);

public:
    void SetBlendFactor(const float r, const float g, const float b, const float a)
    {
        mBlendFactor[0] = a;
        mBlendFactor[1] = r;
        mBlendFactor[2] = g;
        mBlendFactor[3] = b;
    }

protected:
    // Blend State를 만들기 위해 필요한 배열로, 0~7번까지의 렌더 타겟에 대한
    // Blend 방식을 별도로 지정할 수도, 일괄적으로 지정할수도 있다.
    std::vector<D3D11_RENDER_TARGET_BLEND_DESC> mVecDesc;

    // 다중표본화 관련. 32개의 표본 중 추출하지 않을 표본의 비트를 0으로 설정하면 됨
    unsigned int mSampleMask;
    unsigned int mPrevSampleMask;

    // blend Factor
    float mBlendFactor[4];
    float mPrevBlendFactor[4];
};

