#pragma once

#include "../GameInfo.h"

// 렌더를 수행하는 매니저
class CRenderManager
{
public:
	void SetObjList(const std::list<CSharedPtr<class CGameObject>>* pList)
	{
		mpObjList = pList;
	}
	void AddRenderList(class CSceneComponent* pComponent);

public:
	bool Init();
	void Render();

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

	DECLARE_SINGLE(CRenderManager)

public:
	class CStandard2DConstantBuffer* GetStandard2DCBuffer() const
	{
		return mStandard2DBuffer;
	}

private:
	class CRenderStateManager* mRenderStateManager;

	int mRenderCount;
	std::vector<class CSceneComponent*> mVecRender;
	const std::list<CSharedPtr<class CGameObject>>* mpObjList;
	class CStandard2DConstantBuffer* mStandard2DBuffer;
};

