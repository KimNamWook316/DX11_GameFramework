#pragma once

#include "Component.h"

// 씬에서 보여질 수 있는 컴포넌트
class CSceneComponent : public CComponent
{
	friend class CGameObject;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& com);
	virtual ~CSceneComponent();

	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CSceneComponent* Clone();

protected:
	bool mbIsRender; // 렌더 되어야 하는 컴포넌트인지
};

