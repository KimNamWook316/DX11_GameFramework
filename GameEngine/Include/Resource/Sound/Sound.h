#pragma once

#include "../../Ref.h"

class CSound :
    public CRef
{
    friend class CSoundManager;

private:
    CSound();
    ~CSound();

public:
    bool LoadSound(FMOD::System* system, FMOD::ChannelGroup* group,
        bool bLoop, const std::string& soundName, const char* fileName, const std::string& pathName = SOUND_PATH);
    void Play();
    void Stop();
    void Pause();
    void Resume();

public:
    bool IsPlaying() const;

    bool IsLoop() const
    {
        return mbLoop;
    }

private:
    FMOD::System* mSystem;
    FMOD::Sound* mSound;
    FMOD::ChannelGroup* mGroup;
    FMOD::Channel* mChannel;

    bool mbPlay;
    bool mbLoop;
};

