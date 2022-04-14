#pragma once

#include "Component/ObjectComponent.h"
#include "../D2Info.h"

struct EventCallBack
{
    std::string Name;
    std::function<void()> CallBack;
};

class CD2CharacterInfoComponent :
    public CObjectComponent
{
    friend class CGameObject;

protected:
    CD2CharacterInfoComponent();
    CD2CharacterInfoComponent(const CD2CharacterInfoComponent& com);
    virtual ~CD2CharacterInfoComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2CharacterInfoComponent* Clone() override;

public:
    virtual void Save(FILE* fp);
    virtual void Load(FILE* fp);

public:
    void SetData(const std::string& name);
    void SetExp(const int exp);
    void SetSpeed(const float speed);
    void SetDir(const Vector2& dir);
    void SetHp(const float hp);
    void SetMaxHp(const float hp);
    void SetCC(eD2ElementType type);
    void SetMp(const float mp);
    void SetMaxMp(const float mp);
    void SetMeleeBonus(const float val);
    void SetMagicBonus(const float val);
    void SetSpawnRoomIdx(const int idx)
    {
        mSpawnRoomIdx = idx;
    }

public:
    const D2CharInfo& GetInfo() const
    {
        return mCharInfo;
    }

    const std::string& GetName() const
    {
        return mCharInfo.Name;
    }

    const Vector2& GetDir()
    {
        return mCharInfo.Dir;
    }

    float GetSpeed() const
    {
        return mCharInfo.Speed;
    }

    float GetMaxSpeed() const
    {
        return mCharInfo.MaxSpeed;
    }

    float GetHp() const
    {
        return mCharInfo.Hp;
    }

    float GetMaxHp() const
    {
        return mCharInfo.MaxHp;
    }

    float GetHpPercent() const
    {
        return (mCharInfo.Hp / mCharInfo.MaxHp);
    }

    float GetMp() const
    {
        return mCharInfo.Mp;
    }

    float GetMaxMp() const
    {
        return mCharInfo.MaxMp;
    }

    float GetMpPercent() const
    {
        return (mCharInfo.Mp / mCharInfo.MaxMp);
    }

    float GetMeleeBonus() const
    {
        return mCharInfo.MeleeBonus;
    }

    float GetMagicBonus() const
    {
        return mCharInfo.MagicBonus;
    }

    eD2SpriteDir GetSpriteDir() const
    {
        return mCharInfo.eSpriteDir;
    }

    eD2ElementType GetCurCC() const
    {
        return meCC;
    }

    int GetExp() const
    {
        return mCharInfo.Exp;
    }

    int GetMaxExp() const
    {
        return mCharInfo.MaxExp;
    }

    float GetExpPercent() const
    {
        return ((float)mCharInfo.Exp / (float)mCharInfo.MaxExp);
    }

    int GetLevel() const
    {
        return mCharInfo.Level;
    }

    int GetMaxLevel() const
    {
        return mCharInfo.MaxLevel;
    }

    int GetSpawnRoomIdx() const
    {
        return mSpawnRoomIdx;
    }

public:
    template <typename T>
    void SetEventCallBack(const std::string& name, T* obj, void(T::* func)(), eD2CharInfoEventType type)
    {
        EventCallBack eventCallBack;
        eventCallBack.Name = name;
        eventCallBack.CallBack = std::bind(func, obj);

        mEventCallBackList[(int)type].push_back(eventCallBack);
    }

    void DeleteEventCallBack(const std::string& name, eD2CharInfoEventType type);

private:
    void callEventCallBack(eD2CharInfoEventType eType);

private:
    int mSpawnRoomIdx;
    class CSpriteComponent* mSprite;
    class CNavAgentComponent* mNavAgent;
    D2CharInfo mCharInfo;
    std::list<EventCallBack> mEventCallBackList[(int)eD2CharInfoEventType::Max];

    float mPrevSpeed;
    eD2ElementType meCC;
    float mCCTime;
    float mCCTick;
};

