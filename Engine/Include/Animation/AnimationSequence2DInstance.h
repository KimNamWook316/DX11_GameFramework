#pragma once

#include "AnimationSequence2DData.h"

// ��������Ʈ ������Ʈ�� ��� �ִ� ���� �ִϸ��̼� ��ü
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
	class CSpriteComponent* mOwner;													// ���� ������Ʈ
	class CScene* mScene;															// �Ҽ� ��
	std::unordered_map<std::string, CAnimationSequence2DData*> mMapAnimation;		// �ִϸ��̼� ������ ���

};

