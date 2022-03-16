#pragma once

#include "Component/ObjectComponent.h"

class CD2EnemySkillComponent :
    public CObjectComponent
{
    friend class CGameObject;

public:
    CD2EnemySkillComponent();
    CD2EnemySkillComponent(const CD2EnemySkillComponent& com);
    virtual ~CD2EnemySkillComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CD2EnemySkillComponent* Clone() override;

public:
    CGameObject* DoSkill(const std::string& name, const Vector3& startPos, const Vector3& targetPos, 
        const Vector2& dir, class CGameObject* targetObj = nullptr);
};

