#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CPlayer2D :
    public CGameObject
{
    friend class CScene;

protected:
    CPlayer2D();
    CPlayer2D(const CPlayer2D& obj);
    virtual ~CPlayer2D();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CPlayer2D* Clone();
    virtual void Save(FILE* fp) override;
    virtual void Load(FILE* fp) override;

private:
    CSharedPtr<CSpriteComponent> mSprite;
};

