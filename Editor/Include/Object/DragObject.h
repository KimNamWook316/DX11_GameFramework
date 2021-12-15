#pragma once

#include "GameObject\GameObject.h"

class CDragObject :
    public CGameObject
{
    friend class CScene;

protected:
    CDragObject();
    CDragObject(const CDragObject& obj);
    virtual ~CDragObject();

public: 
    virtual bool Init() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual CDragObject* Clone();

public:
    void SetStartPos(const Vector2& pos);
    void SetEndPos(const Vector2& pos);

private:
    CSharedPtr<class CStaticMeshComponent> mMeshComponent;
    Vector2 mStartPos;
    Vector2 mEndPos;
};

