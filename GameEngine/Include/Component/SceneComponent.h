#pragma once

#include "Component.h"
#include "Transform.h"

// 씬에서 보여질 수 있는 컴포넌트
class CSceneComponent : public CComponent
{
	friend class CGameObject;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& com);
	virtual ~CSceneComponent();
	
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CSceneComponent* Clone();

public:
	virtual void SetScene(class CScene* scene);
	virtual void SetGameObject(class CGameObject* obj);

public:
	void AddChild(CSceneComponent* child);
	bool DeleteChild(CSceneComponent* deleteTarget);
	bool DeleteChild(std::string& deleteTargetName);
	CSceneComponent* FindComponent(const std::string& name);

protected:
	bool mbIsRender; // 렌더 되어야 하는 컴포넌트인지
	CTransform* mTransform;	// 씬에 보여지는 컴포넌트이므로 위치 정보는 무조건 있어야 한다.
	CSceneComponent* mParent; // 상위 컴포넌트
	std::vector<CSharedPtr<CSceneComponent>> mVecChild; // 자식 컴포넌트
};

