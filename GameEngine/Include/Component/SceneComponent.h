#pragma once

#include "Component.h"
#include "Transform.h"

// ������ ������ �� �ִ� ������Ʈ
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
	bool mbIsRender; // ���� �Ǿ�� �ϴ� ������Ʈ����
	CTransform* mTransform;	// ���� �������� ������Ʈ�̹Ƿ� ��ġ ������ ������ �־�� �Ѵ�.
	CSceneComponent* mParent; // ���� ������Ʈ
	std::vector<CSharedPtr<CSceneComponent>> mVecChild; // �ڽ� ������Ʈ
};

