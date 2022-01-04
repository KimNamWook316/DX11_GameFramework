#pragma once

#include "GameObject/GameObject.h"

class CCameraObject :
    public CGameObject
{
    friend class CScene;

protected:
    CCameraObject();
    CCameraObject(const CCameraObject& obj);
    virtual ~CCameraObject();

public: 
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CCameraObject* Clone();

private:
    CSharedPtr<class CCameraComponent> mCamera;
};

