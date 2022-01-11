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

�ý����� ä�� �׷��� ����

ä���� ��������� ����ȴ�.
���带 ä���� ������ �ְ� ��������ش�.

ä�� �׷���?
fmod �ý����� ������� ���, �����ͱ׷��� �ϳ� ���������.
�ƹ� �͵� �� �ϸ� ������ �׷��� ��ü ä���� ��� ��Ʈ���Ѵ�.

�Ϲ� ���� �׷�, ����Ʈ, bgm �׷� �� ����� ���� �׷� ����� ����
*/
