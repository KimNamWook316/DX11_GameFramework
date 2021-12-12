#pragma once

#include "GameObject\GameObject.h"

class CSpriteEditObject :
    public CGameObject
{
    friend class CScene;

protected:
    CSpriteEditObject();
    CSpriteEditObject(const CSpriteEditObject& obj);
    virtual ~CSpriteEditObject();

public:
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CSpriteEditObject* Clone();

public:
    class CSpriteComponent* GetSpriteComponent() const
    {
        return mSprite;
    }

private:
    CSharedPtr<class CSpriteComponent> mSprite;
};

