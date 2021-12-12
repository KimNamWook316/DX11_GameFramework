#pragma once

#include "AnimationSequence2DData.h"

// 스프라이트 컴포넌트가 들고 있는 실제 애니메이션 객체
class CAnimationSequence2DInstance
{
	friend class CSpriteComponent;

protected:
	CAnimationSequence2DInstance();
	CAnimationSequence2DInstance(const CAnimationSequence2DInstance& anim);
	virtual ~CAnimationSequence2DInstance();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	void SetShader();
	void ResetShader();
	virtual CAnimationSequence2DInstance* Clone();

public:
	int GetAnimationCount() const
	{
		return (int)mMapAnimation.size();
	}

public:
	void AddAnimation(const std::string& sequenceName, const std::string& name, bool bIsLoop = true,
		const float playTime = 1.f, const float playScale = 1.f, bool bIsReverse = false);
	void ChangeAnimation(const std::string& name);
	bool CheckCurrentAnimation(const std::string& name);

public:
	void SetPlayTime(const std::string& name, const float playTime);
	void SetPlayScale(const std::string& name, const float playScale);
	void SetLoop(const std::string& name, bool bLoop);
	void SetReverse(const std::string& name, bool bReverse);
	void SetCurrentAnimation(const std::string& name);

public:
	void SetScene(class CScene* scene)
	{
		mScene = scene;
	}

	void SetOwner(class CSpriteComponent* owner)
	{
		mOwner = owner;
	}

public:
	template <typename T>
	void SetEndCallBack(const std::string& name, T* obj, void(T::* func)())
	{
		CAnimationSequence2DData* data = findAnimation(name);

		if (!data)
		{
			return;
		}

		data->SetEndCallBack<T>(obj, func);
	}

	template <typename T>
	void AddNotify(const std::string& name, const int frame, T* obj, void(T::* func)())
	{
		CAnimationSequence2DData* data = findAnimation(name);

		if (!data)
		{
			return;
		}

		data->AddNotify<T>(name, frame, obj, func);
	}

protected:
	CAnimationSequence2DData* findAnimation(const std::string& name);

protected:
	class CSpriteComponent* mOwner;													// 주인 컴포넌트
	class CScene* mScene;															// 소속 씬
	CAnimationSequence2DData* mCurrentAnimation;									// 현재 애니메이션
	std::unordered_map<std::string, CAnimationSequence2DData*> mMapAnimation;		// 애니메이션 데이터 목록
	class CAnimation2DConstantBuffer* mCBuffer;
};

