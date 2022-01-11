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

public:
	bool LoadSound(const std::string& channelGroupName, bool bLoop, const std::string& soundName,
		const char* fileName, const std::string& pathName = SOUND_PATH);
	bool CreateSoundChannelGroup(const std::string& groupName);
	bool SetVolume(int volume);											// 0 ~ 100
	bool SetVolume(const std::string& groupName, int volume);
	bool SoundPlay(const std::string& soundName);
	bool SoundStop(const std::string& soundName);
	bool SoundPause(const std::string& soundName);
	bool SoundResume(const std::string& soundName);
	CSound* FindSound(const std::string& soundName);
	FMOD::ChannelGroup* FindChannelGroup(const std::string& groupName);
	void ReleaseSound(const std::string& soundName);

private:
	FMOD::System* mSystem;
	FMOD::ChannelGroup* mMasterGroup;
	std::unordered_map<std::string, FMOD::ChannelGroup*> mMapGroup;
	std::unordered_map<std::string, CSharedPtr<CSound>> mMapSound;
};

/*
FMOD

시스템이 채널 그룹을 관리

채널이 만들어지고 재생된다.
사운드를 채널이 가지고 있고 재생시켜준다.

채널 그룹은?
fmod 시스템을 만들었을 경우, 마스터그룹이 하나 만들어진다.
아무 것도 안 하면 마스터 그룹이 전체 채널을 모두 컨트롤한다.

일반 사운드 그룹, 이펙트, bgm 그룹 등 사용자 정의 그룹 만들기 가능
*/
