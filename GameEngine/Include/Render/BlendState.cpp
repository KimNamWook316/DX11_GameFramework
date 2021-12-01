#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState()	:
	mSampleMask(0xffffffff),	// 기본값
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
	// 이전 상태를 받아오고, 현재 상태로 설정
	CDevice::GetInst()->GetContext()->OMGetBlendState((ID3D11BlendState**)&mPrevState, mPrevBlendFactor, &mPrevSampleMask);
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)&mState, mBlendFactor, mSampleMask);
}

void CBlendState::ResetState()
{
	// 이전 상태로 되돌린다.
	CDevice::GetInst()->GetContext()->OMSetBlendState((ID3D11BlendState*)&mPrevState, mPrevBlendFactor, mPrevSampleMask);
}

void CBlendState::AddBlendInfo(bool bIsBlendEnalbe, D3D11_BLEND srcBlend, 
	D3D11_BLEND destBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND srcBlendAlpha,
	D3D11_BLEND destBlendAlpha, D3D11_BLEND_OP blendOpAlpha, 
	UINT8 renderTargetWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC desc = {};

	desc.BlendEnable = bIsBlendEnalbe;	// Blend 할 것인지
	desc.SrcBlend = srcBlend;			// 원본 Blend Factor
	desc.DestBlend = destBlend;			// 혼합의 대상 Blend Factor
	desc.BlendOp = blendOp;				// 혼합 연산자
	desc.SrcBlendAlpha = srcBlendAlpha;
	desc.DestBlendAlpha = destBlendAlpha;
	desc.BlendOpAlpha = blendOpAlpha;
	desc.RenderTargetWriteMask = renderTargetWriteMask;	// 후면 버퍼의 어떤 색상 채널에 기록할 것인가

	mVecDesc.push_back(desc);	
}

bool CBlendState::CreateState(bool bIsAlphaToCoverageEnable, bool bIsIndependentBlendEnable)
{
	if (mVecDesc.empty())
	{
		return false;
	}

	D3D11_BLEND_DESC desc = {};
	
	desc.AlphaToCoverageEnable = bIsAlphaToCoverageEnable;		// multi-sampling의 alpha-to-coverage 관련
	desc.IndependentBlendEnable = bIsIndependentBlendEnable;	// 각 Render Target마다 blend state를 따로 지정할 것인지

	// blend description을 복사한다.
	memcpy(desc.RenderTarget, &mVecDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * mVecDesc.size());
	
	// blend state 생성
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBlendState(&desc, (ID3D11BlendState**)&mState)))
	{
		assert(false);
		return false;
	}

	return true;
}
