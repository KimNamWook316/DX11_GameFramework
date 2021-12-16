#pragma once

#include "../GameInfo.h"

// �ִϸ��̼� ��� �� ������ �ݹ� ������ ��� �ִ� ����ü
struct Animation2DNotify
{
	std::string Name;
	int Frame;							// �� �����ӿ� ������ ������
	bool bIsCalled;						// ����Ǿ����� 
	std::function<void()> CallBack;		// �ݹ� �Լ�

	Animation2DNotify() :
		Frame(0),
		bIsCalled(false)
	{
	};
};

class CAnimationSequence2DData
{
	friend class CAnimationSequence2DInstance;

private:
	CAnimationSequence2DData();
	~CAnimationSequence2DData();

public:
	void Replay();

public:
	int GetCurrentFrame() const
	{
		return mFrame;
	}

	float GetAnimationTime() const
	{
		return mTime;
	}

	class CAnimationSequence2D* GetAnimationSequence() const
	{
		return mSequence;
	}

public:
	template <typename T>
	void SetEndCallBack(T* obj, void(T::* func)())
	{
		mEndCallBack = std::bind(func, obj);
	}

	template <typename T>
	void AddNotify(const std::string& name, const int frame, T* obj, void(T::* func)())
	{
		Animation2DNotify* notify = new Animation2DNotify;

		notify->Name = name;
		notify->Frame = frame;
		notify->CallBack = std::bind(func, obj);

		mVecNotify.push_back(notify);
	}

private:
	std::string mName;										// �� �ִϸ��̼��� �̸�
	CSharedPtr<class CAnimationSequence2D> mSequence;		// �ִϸ��̼� ����
	int mFrame;												// �� ������ ¥������	
	float mTime;											// ����ð�
	float mFrameTime;										// �����Ӵ� ����ð�
	float mPlayTime;										// �� ����ð�
	float mPlayScale;										// ��� ���
	bool mbIsLoop;											// �ݺ��Ǵ� �ִϸ��̼�����
	bool mbIsReverse;										// ���������
	std::function<void()> mEndCallBack;						// �ִϸ��̼��� ������ ȣ���ϴ� �ݹ�
	std::vector<Animation2DNotify*> mVecNotify;				// �ݹ� ���
};
