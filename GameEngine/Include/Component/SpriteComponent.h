#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Resource/Material/Material.h"
#include "../Animation/AnimationSequence2DInstance.h"

class CSpriteComponent : public CSceneComponent
{
	friend class CGameObject;

protected:
	CSpriteComponent();
	CSpriteComponent(const CSpriteComponent& com);
	virtual ~CSpriteComponent();

public:
	void SetMaterial(CMaterial* material);

public:
	CMaterial* GetMaterial()
	{
		return mMaterial;
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

public:
	virtual void Save(FILE* fp);
	virtual void Load(FILE* fp);

public:
	void SetBaseColor(const Vector4& color);
	void SetBaseColor(const float r, const float g, const float b, const float a);
	void SetRenderState(class CRenderState* state);
	void SetRenderState(const std::string& name);
	void SetTransparency(bool bEnable);
	void SetOpacity(const float val);
	void AddOpacity(const float val);

public:
	void AddTexture(const int reg, const int shaderType, const std::string& name,
		class CTexture* texture);
	void AddTexture(const int reg, const int shaderType, const std::string& name,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void AddTextureFullPath(const int reg, const int shaderType, const std::string& name,
		const TCHAR* fullPath);
	void AddTexture(const int reg, const int shaderType, const std::string& name,
		const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);

	void SetTexture(const int index, const int reg, const int shaderType,
		const std::string& name, class CTexture* texture);
	void SetTexture(const int index, const int reg, const int shaderType,
		const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void SetTextureFullPath(const int index, const int reg, const int shaderType,
		const std::string& name, const TCHAR* fullPath);
	void SetTexture(const int index, const int reg, const int shaderType,
		const std::string& name, const std::vector<TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);

public:
	void ChangeAnimation(const std::string& name);
	void AddAnimation(const std::string& sequenceName, const std::string& name, bool bIsLoop = true,
		const float playTime = 1.f, const float playScale = 1.f, bool bIsReverse = false);
	bool CheckCurrentAnimation(const std::string& name);

public:
	void SetPlayTime(const std::string& name, const float playTime);
	void SetPlayScale(const std::string& name, const float playScale);
	void SetLoop(const std::string& name, bool bLoop);
	void SetReverse(const std::string& name, bool bReverse);
	void SetCurrentAnimation(const std::string& name);

public:
	template <typename T>
	void CreateAnimationInstance()
	{
		T* anim = new T;

		anim->SetScene(mScene);
		anim->SetOwner(this);

		if (!anim->Init())
		{
			SAFE_DELETE(anim);
			assert(false);
			return;
		}

		// 원래 애니메이션이 있으면 삭제
		SAFE_DELETE(mAnimation);

		mAnimation = anim;
	}

	template <typename T>
	void LoadAnimationInstance()
	{
		T* anim;
		anim->SetScene(mScene);
		anim->SetOwner(this);
		SAFE_DELETE(mAnimation);
		mAnimation = anim;
	}

	template <typename T>
	void AddNotify(const std::string& name, const int frame, T* obj, void(T::* func)())
	{
		if (mAnimation)
		{
			mAnimation->AddNotify<T>(name, frame, obj, func);
		}
	}

	template <typename T>
	void SetEndCallBack(const std::string& name, T* obj, void(T::* func)())
	{
		if (mAnimation)
		{
			mAnimation->SetEndCallBack<T>(name, obj, func);
		}
	}

protected:
	CSharedPtr<CSpriteMesh> mMesh;
	CSharedPtr<CMaterial> mMaterial;
	class CAnimationSequence2DInstance* mAnimation;
};

