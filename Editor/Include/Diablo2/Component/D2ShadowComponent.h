#pragma once

#include "Component/SceneComponent.h"
#include "Resource/Material/Material.h"

class CD2ShadowComponent :
    public CSceneComponent 
{
    friend class CGameObject;

protected:
	CD2ShadowComponent();
	CD2ShadowComponent(const CD2ShadowComponent& com);
	virtual ~CD2ShadowComponent();
	
public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void Render() override;
	virtual CD2ShadowComponent* Clone() override;

public:
	virtual void Save(FILE* fp) override;
	virtual void Load(FILE* fp) override;

public:
	void SetOffset(const Vector2& offset)
	{
		mOffset = offset;
	}

public:
	const Vector2& GetOffset() const
	{
		return mOffset;
	}

private:
	class CSpriteComponent* mSprite;
	CSharedPtr<class CMesh> mMesh;
	CTexture* mTexture;
	CSharedPtr<class CGraphicShader> mShader;
	class CAnimationSequence2DInstance* mAnimation;
	Vector2 mOffset;
};

