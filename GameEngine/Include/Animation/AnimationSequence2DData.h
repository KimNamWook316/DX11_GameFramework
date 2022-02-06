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
	void Save(FILE* fp);
	void Load(FILE* fp);

public:
	const std::string& GetName() const
	{
		return mName;
	}

	bool IsLoop() const
	{
		return mbIsLoop;
	}

	bool IsReverse() const
	{
		return mbIsReverse;
	}

	float GetPlayTime() const
	{
		return mPlayTime;
	}

	float GetPlayScale() const
	{
		return mPlayScale;
	}

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
	void SetPlayTime(const float time)
	{
		mPlayTime = time;
	}

	void SetPlayScale(const float scale)
	{
		mPlayScale = scale;
	}

	void EnableLoop(bool bLoop)
	{
		mbIsLoop = bLoop;
	}

	void EnableReverse(bool bRev)
	{
		mbIsReverse = bRev;
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
	std::string mSequenceName;
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

