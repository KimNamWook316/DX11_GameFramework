#pragma once

#include "../GameInfo.h"

// 애니메이션 재생 중 실행할 콜백 정보를 담고 있는 구조체
struct Animation2DNotify
{
	std::string Name;
	int Frame;							// 몇 프레임에 실행할 것인지
	bool bIsCalled;						// 실행되었는지 
	std::function<void()> CallBack;		// 콜백 함수

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
	std::string mName;										// 이 애니메이션의 이름
	std::string mSequenceName;
	CSharedPtr<class CAnimationSequence2D> mSequence;		// 애니메이션 정보
	int mFrame;												// 몇 프레임 짜리인지	
	float mTime;											// 실행시간
	float mFrameTime;										// 프레임당 실행시간
	float mPlayTime;										// 총 실행시간
	float mPlayScale;										// 재생 배수
	bool mbIsLoop;											// 반복되는 애니메이션인지
	bool mbIsReverse;										// 역재생인지
	std::function<void()> mEndCallBack;						// 애니메이션이 끝나면 호출하는 콜백
	std::vector<Animation2DNotify*> mVecNotify;				// 콜백 목록
};

