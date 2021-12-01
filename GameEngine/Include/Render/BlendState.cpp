#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState()	:
	mSampleMask(0xffffffff),	// �⺻��
	mPrevSampleMask(0),
	mBlendFactor{},
	mPrevBlendFactor{}
{
}

CBlendState::~CBlendState()
{
}

void CBlendState::SetState()
{
	// ���� ���¸� �޾ƿ���, ���� ���·� ����
	CDevice::GetInst()->GetContext()->OMGetBlendState((ID3D11BlendState**)&mPrevState, mPrevBlendFactor, &mPrevSampleMask);
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)&mState, mBlendFactor, mSampleMask);
}

void CBlendState::ResetState()
{
	// ���� ���·� �ǵ�����.
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)&mPrevState, mPrevBlendFactor, mPrevSampleMask);
}

void CBlendState::AddBlendInfo(bool bIsBlendEnalbe, D3D11_BLEND srcBlend, 
	D3D11_BLEND destBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND srcBlendAlpha,
	D3D11_BLEND destBlendAlpha, D3D11_BLEND_OP blendOpAlpha, 
	UINT8 renderTargetWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC desc = {};

	desc.BlendEnable = bIsBlendEnalbe;	// Blend �� ������
	desc.SrcBlend = srcBlend;			// ���� Blend Factor
	desc.DestBlend = destBlend;			// ȥ���� ��� Blend Factor
	desc.BlendOp = blendOp;				// ȥ�� ������
	desc.SrcBlendAlpha = srcBlendAlpha;
	desc.DestBlendAlpha = destBlendAlpha;
	desc.BlendOpAlpha = blendOpAlpha;
	desc.RenderTargetWriteMask = renderTargetWriteMask;	// �ĸ� ������ � ���� ä�ο� ����� ���ΰ�

	mVecDesc.push_back(desc);	
}

bool CBlendState::CreateState(bool bIsAlphaToCoverageEnable, bool bIsIndependentBlendEnable)
{
	if (mVecDesc.empty())
	{
		return false;
	}

	D3D11_BLEND_DESC desc = {};
	
	desc.AlphaToCoverageEnable = bIsAlphaToCoverageEnable;		// multi-sampling�� alpha-to-coverage ����
	desc.IndependentBlendEnable = bIsIndependentBlendEnable;	// �� Render Target���� blend state�� ���� ������ ������

	// blend description�� �����Ѵ�.
	memcpy(desc.RenderTarget, &mVecDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * mVecDesc.size());
	
	// blend state ����
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&desc, (ID3D11BlendState**)&mState)))
	{
		assert(false);
		return false;
	}

	return true;
}
