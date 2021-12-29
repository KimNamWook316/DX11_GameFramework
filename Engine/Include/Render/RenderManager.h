#pragma once

#include "../GameInfo.h"

struct RenderLayer
{
	std::string Name;
	int LayerPriority;
	std::vector<class CSceneComponent*> RenderList; // 이 레이어에서 렌더되는 오브젝트들 
	int RenderCount;								

	RenderLayer()
	{
		LayerPriority = 0;
		RenderCount = 0;
		RenderList.resize(500);
	}
};

// 렌더를 수행하는 매니저
class CRenderManager
{
public:
	void SetObjList(const std::list<CSharedPtr<class CGameObject>>* pList)
	{
		mpObjList = pList;
	}
	void AddRenderList(class CSceneComponent* pComponent);
	void CreateLayer(const std::string& name, const int priority);
	void SetLayerPriority(const std::string& name, const int priority);

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
	static bool sortLayer(RenderLayer* src, RenderLayer* dest);

private:
	class CRenderStateManager* mRenderStateManager;

	int mRenderCount;
	std::vector<RenderLayer*> mRenderLayerList;
	const std::list<CSharedPtr<class CGameObject>>* mpObjList;
	class CStandard2DConstantBuffer* mStandard2DBuffer;
	
	// 2D용 Depth 사용 하지 않는 렌더링 스테이트
	class CRenderState* mDepthDisableState;
};

