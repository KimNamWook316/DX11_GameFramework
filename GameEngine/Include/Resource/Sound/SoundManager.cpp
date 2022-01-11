#include "SoundManager.h"

CSoundManager::CSoundManager()	:
	mSystem(nullptr),
	mMasterGroup(nullptr)
{
}

CSoundManager::~CSoundManager()
{
	auto iter = mMapGroup.begin();
	auto iterEnd = mMapGroup.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->release();
	}

	mMapSound.clear();

	mSystem->close();
	mSystem->release();
}

bool CSoundManager::Init()
{
	FMOD_RESULT result = FMOD::System_Create(&mSystem);
	
	if (result != FMOD_OK)
	{
		assert(false);
		return false;
	}

	result = mSystem->init(4093, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
	{
		assert(false);
		return false;
	}

	// Master Group 가져오기
	result = mSystem->getMasterChannelGroup(&mMasterGroup);

	if (result != FMOD_OK)
	{
		assert(false);
		return false;
	}

	mMapGroup.insert(std::make_pair("Master", mMasterGroup));

	return true;
}

void CSoundManager::Update()
{
	mSystem->update();
}

bool CSoundManager::LoadSound(const std::string& channelGroupName, bool bLoop, const std::string& soundName, const char* fileName, const std::string& pathName)
{
	CSound* sound = FindSound(soundName);

	if (sound)
	{
		return true;
	}

	FMOD::ChannelGroup* group = FindChannelGroup(channelGroupName);

	sound = new CSound;

	if (!sound->LoadSound(mSystem, group, bLoop, soundName, fileName, pathName))
	{
		assert(false);
		return false;
	}

	mMapSound.insert(std::make_pair(soundName, sound));
	return true;
}

bool CSoundManager::CreateSoundChannelGroup(const std::string& groupName)
{
	FMOD::ChannelGroup* group = FindChannelGroup(groupName);

	if (group)
	{
		return true;
	}

	FMOD_RESULT result = mSystem->createChannelGroup(groupName.c_str(), &group);
	
	if (FMOD_OK != result)
	{
		assert(false);
		return false;
	}

	// Master Group에서 모든 채널그룹을 관리한다.
	mMasterGroup->addGroup(group);

	mMapGroup.insert(std::make_pair(groupName, group));
}

bool CSoundManager::SetVolume(int volume)
{
	FMOD_RESULT result = mMasterGroup->setVolume(volume / 100.f);
	return FMOD_OK == result;
}

bool CSoundManager::SetVolume(const std::string& groupName, int volume)
{
	FMOD::ChannelGroup* group = FindChannelGroup(groupName);

	if (!group)
	{
		return false;
	}

	FMOD_RESULT result = group->setVolume(volume / 100.f);
	return FMOD_OK == result;
}

bool CSoundManager::SoundPlay(const std::string& soundName)
{
	CSound* sound = FindSound(soundName);

	if (!sound)
	{
		return false;
	}

	sound->Play();
	return true;
}

bool CSoundManager::SoundStop(const std::string& soundName)
{
	CSound* sound = FindSound(soundName);

	if (!sound)
	{
		return false;
	}

	sound->Stop();
	return true;
}

bool CSoundManager::SoundPause(const std::string& soundName)
{
	CSound* sound = FindSound(soundName);

	if (!sound)
	{
		return false;
	}

	sound->Pause();
	return true;
}

bool CSoundManager::SoundResume(const std::string& soundName)
{
	CSound* sound = FindSound(soundName);

	if (!sound)
	{
		return false;
	}

	sound->Resume();
	return true;
}

CSound* CSoundManager::FindSound(const std::string& soundName)
{
	auto iter = mMapSound.find(soundName);

	if (iter == mMapSound.end())
	{
		return nullptr;
	}
	return iter->second;
}

FMOD::ChannelGroup* CSoundManager::FindChannelGroup(const std::string& groupName)
{
	auto iter = mMapGroup.find(groupName);

	if (iter == mMapGroup.end())
	{
		return nullptr;
	}
	return iter->second;
}

void CSoundManager::ReleaseSound(const std::string& soundName)
{
	auto iter = mMapSound.find(soundName);

	if (iter != mMapSound.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			mMapSound.erase(iter);
		}
	}
}
