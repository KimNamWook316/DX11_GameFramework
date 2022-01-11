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
