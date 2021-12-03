#pragma once

#include "AnimationSequence2DData.h"

// 스프라이트 컴포넌트가 들고 있는 실제 애니메이션 객체
class CAnimationSequence2DInstance
{
	friend class CSpriteComponent;

private:
	CAnimationSequence2DInstance();
	CAnimationSequence2DInstance(const CAnimationSequence2DInstance& anim);
	~CAnimationSequence2DInstance();

public:
	void Update(float deltaTime);
	CAnimationSequence2DInstance* Clone();

public:
	void AddAnimation(const std::string& sequenceName, const std::string& name, bool bIsLoop,
		const float playTime, const float playScale, bool bIsReverse);
	void ChangeAnimation(const std::string& name);
	bool CheckCurrentAnimation(const std::string& name);

public:
	void SetPlayTime(const std::string& name, const float playTime);
	void SetPlayScale(const std::string& name, const float playScale);
	void SetLoop(const std::string& name, bool bLoop);
	void SetReverse(const std::string& name, bool bReverse);
	void SetCurrentAnimation(const std::string& name);

public:
	template <typename T>
	void SetEndCallBack(const std::string& name, T* obj, void(T::* func)())
	{
		CAnimationSequence2DData* data = FindAnimation(name);

		if (!data)
		{
			return;
		}

		data->SetEndCallBack<T>(obj, func);
	}

	template <typename T>
	void AddNotify(const std::string& name, const int frame, T* obj, void(T::* func)())
	{
		CAnimationSequence2DData* data = FindAnimation(name);

		if (!data)
		{
			return;
		}

		data->AddNotify<T>(name, frame, obj, func);
	}

private:
	CAnimationSequence2DData* FindAnimation(const std::string& name);

private:
	class CSpriteComponent* mOwner;													// 주인 컴포넌트
	class CScene* mScene;															// 소속 씬
	std::unordered_map<std::string, CAnimationSequence2DData*> mMapAnimation;		// 애니메이션 데이터 목록

};

