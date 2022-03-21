#pragma once

#include "Component/ObjectComponent.h"
#include "../D2Flag.h"
#include "D2Skill.h"

enum eCSVLabel
{
    Name,
    AttackType,
    TreeNo,
    MaxLevel,
    IsDefault,
    ParentSkill,
    PreSkillLevel,
    Mp,
    Range,
};

struct Skill
{
    std::string Name;
    eD2AttackType Type;
    CSharedPtr<CGameObject> Object;
};

class CD2PlayerSkillComponent :
    public CObjectComponent
{
    friend class CGameObject;

public:
    CD2PlayerSkillComponent();
    CD2PlayerSkillComponent(const CD2PlayerSkillComponent& com);
    virtual ~CD2PlayerSkillComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2PlayerSkillComponent* Clone() override;

public:
    void LevelUpAll(float deltaTime);  // DEBUG
    bool LevelUp(eD2SkillTreeNo treeNo, const std::string& skillName);

public:
    int GetRSkillType();
    int GetRSkillElementType();

public:
    void SetNextRSkill(float deltaTime); // DEBUG
    void SetRSkill(const int idx);
    void SetRSkill(const std::string& name);

public:
    float GetRSkillMp()
    {
        if (!isValidIdx(mRSkillIdx))
        {
            return -1;
        }

        return mVecAvailableSkill[mRSkillIdx]->GetMp();
    }

    float GetRSkillRange()
    {
        if (!isValidIdx(mRSkillIdx))
        {
            return -1;
        }

        return mVecAvailableSkill[mRSkillIdx]->GetRange();
    }

public:
    CGameObject* DoMeleeAttack(const Vector3& startPos, const Vector3& targetPos, const Vector2& dir, class CGameObject* targetObj = nullptr);
    CGameObject* DoRSkill(const Vector3& startPos, const Vector3& targetPos, const Vector2& dir, class CGameObject* targetObj = nullptr);

public:
    void AddActiveSkill(CD2Skill* skill)
    {
        mVecAvailableSkill.push_back(skill);
    }

public:
    const std::string& GetCurrentSkillName()
    {
        return mVecAvailableSkill[mRSkillIdx]->mName;
    }

private:
    bool isValidIdx(const int idx);
    bool loadSkillList();
    CD2Skill* findSkill(const std::string& name);
    int findSkillIdx(const std::string& name);

private:
    int mMeleeAttackIdx;
    int mRSkillIdx;

    // Skill Tree
    bool mbInit;
    CD2Skill* mMeleeAtack;
    CD2Skill* mArrSkillTree[(int)eD2SkillTreeNo::Max];
    std::vector<CD2Skill*> mVecAvailableSkill;
};
