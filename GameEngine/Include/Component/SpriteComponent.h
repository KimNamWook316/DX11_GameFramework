#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Resource/Material/Material.h"

class CSpriteComponent : public CSceneComponent
{
	friend class CGameObject;

protected:
	CSpriteComponent();
	CSpriteComponent(const CSpriteComponent& com);
	virtual ~CSpriteComponent();

public:
	void SetMaterial(CMaterial* Material)
	{
		mMaterial = Material;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CSpriteComponent* Clone();

protected:
	CSharedPtr<CSpriteMesh> mMesh;
	CSharedPtr<CMaterial> mMaterial;
};

