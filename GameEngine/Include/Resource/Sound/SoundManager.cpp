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
}
