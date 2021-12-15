#include "DepthStencilState.h"
#include "../Device.h"

CDepthStencilState::CDepthStencilState()	:
	mStencilRef(0xffffffff),
	mPrevStencilRef(0xffffffff)
{
	meStateType = eRenderStateType::DepthStencil;
}

CDepthStencilState::~CDepthStencilState()
{
}

bool CDepthStencilState::CreateState(bool bDepthEnable, D3D11_DEPTH_WRITE_MASK depthWriteMask, D3D11_COMPARISON_FUNC depthFunc, bool bStencilEnable, UINT8 stencilReadMask, UINT8 stencilWriteMask, D3D11_DEPTH_STENCILOP_DESC frontFace, D3D11_DEPTH_STENCILOP_DESC backFace)
{
	D3D11_DEPTH_STENCIL_DESC desc = {};

	desc.DepthEnable = bDepthEnable;
	desc.DepthWriteMask = depthWriteMask;
	desc.DepthFunc = depthFunc;
	desc.StencilEnable = bStencilEnable;
	desc.StencilReadMask = stencilReadMask;
	desc.StencilWriteMask = stencilWriteMask;
	desc.FrontFace = frontFace;
	desc.BackFace = backFace;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDepthStencilState(&desc,
		(ID3D11DepthStencilState**)&mState)))
	{
		assert(false);
		return false;
	}

	return true;
}

void CDepthStencilState::SetState()
{
	CDevice::GetInst()->GetContext()->OMGetDepthStencilState((ID3D11DepthStencilState**)&mPrevState, &mPrevStencilRef);
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)mState, mStencilRef);
}

void CDepthStencilState::ResetState()
{
	CDevice::GetInst()->GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)mPrevState, mPrevStencilRef);
	SAFE_RELEASE(mPrevState);
}
