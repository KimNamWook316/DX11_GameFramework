#pragma once

#include "Component/ObjectComponent.h"
#include "../D2Flag.h"

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
    int GetLSkillType();
    int GetRSkillType();

public:
    void SetLSkill(const int idx);
    void SetLSkill(const std::string& name);
    void SetRSkill(const int idx);
    void SetRSkill(const std::string& name);

public:
    CGameObject* DoLSkill(const Vector3& startPos);
    CGameObject* DoRSkill(const Vector3& startPos);

private:
    bool isValidIdx(const int idx);
    bool loadSkillList();
    Skill* findSkill(const std::string& name);
    int findSkillIdx(const std::string& name);

public:
    int mLSkillIdx;
    int mRSkillIdx;
    std::vector<Skill*> mVecSkill;
};

