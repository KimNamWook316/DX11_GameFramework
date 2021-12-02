#pragma once

#include "RenderState.h"

// ������ �߰� ������ �ϱ� ������, �ʿ� ���� ���� ���� �ʾƾ� �Ѵ�.
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
    // Blend State�� ����� ���� �ʿ��� �迭��, 0~7�������� ���� Ÿ�ٿ� ����
    // Blend ����� ������ ������ ����, �ϰ������� �����Ҽ��� �ִ�.
    std::vector<D3D11_RENDER_TARGET_BLEND_DESC> mVecDesc;

    // ����ǥ��ȭ ����. 32���� ǥ�� �� �������� ���� ǥ���� ��Ʈ�� 0���� �����ϸ� ��
    unsigned int mSampleMask;
    unsigned int mPrevSampleMask;

    // blend Factor
    float mBlendFactor[4];
    float mPrevBlendFactor[4];
};

