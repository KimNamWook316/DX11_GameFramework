#pragma once

#include "../../GameInfo.h"
#include "Sound.h"

class CSoundManager
{
	friend class CResourceManager;

private:
	CSoundManager();
	~CSoundManager();

public:
	bool Init();
	void Update();

private:
	FMOD::System* mSystem;
	FMOD::ChannelGroup* mMasterGroup;
	std::unordered_map<std::string, FMOD::ChannelGroup*> mMapGroup;
	std::unordered_map<std::string, CSharedPtr<CSound>> mMapSound;
};

