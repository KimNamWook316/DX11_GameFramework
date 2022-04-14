#include "Sound.h"
#include "../../PathManager.h"

CSound::CSound()	:
	mSystem(nullptr),
	mSound(nullptr),
	mGroup(nullptr),
	mChannel(nullptr),
	mbPlay(false),
	mbLoop(false)
{
}

CSound::~CSound()
{
	if (mSound)
	{
		mSound->release();
	}
}

bool CSound::LoadSound(FMOD::System* system, FMOD::ChannelGroup* group, bool bLoop, const std::string& soundName, const char* fileName, const std::string& pathName)
{
	SetName(soundName);
	mSystem = system;
	mGroup = group;
	mbLoop = bLoop;

	char fullPath[MAX_PATH] = {};
	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
	{
		strcpy_s(fullPath, info->PathMultibyte);
	}
	strcat_s(fullPath, fileName);

	FMOD_MODE mode = FMOD_DEFAULT;

	if (mbLoop)
	{
		mode = FMOD_LOOP_NORMAL;
	}

	if (FMOD_OK != mSystem->createSound(fullPath, mode, nullptr, &mSound))
	{
		assert(false);
		return false;
	}
	return true;
}

void CSound::Play()
{
	// mChannel에 현재 채널을 받아오며 재생시킨다.
	mSystem->playSound(mSound, mGroup, false, &mChannel);
	mbPlay = true;
}

void CSound::Stop()
{
	if (mChannel)
	{
		// 현재 채널이 재생중인지 받아온다.
		bool bPlaying = false;
		mChannel->isPlaying(&bPlaying);

		if (bPlaying)
		{
			mChannel->stop();
			mChannel = nullptr;
			mbPlay = false;
		}
	}
}

void CSound::Pause()
{
	if (mChannel)
	{
		bool bPlaying = false;
		mChannel->isPlaying(&bPlaying);

		if (bPlaying)
		{
			mChannel->setPaused(true);
		}
		mbPlay = false;
	}
}

void CSound::Resume()
{
	if (mChannel)
	{
		bool bPlaying = false;
		mChannel->isPlaying(&bPlaying);

		if (!bPlaying)
		{
			mChannel->setPaused(false);
		}
		mbPlay = true;
	}
}

bool CSound::IsPlaying() const
{
	if (mChannel)
	{
		bool bPlaying = false;
		mChannel->isPlaying(&bPlaying);

		return bPlaying;
	}

	return false;
}
