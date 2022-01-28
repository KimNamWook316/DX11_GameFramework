#pragma once

#include "GameObject/GameObject.h"

class CBubbleParticle :
    public CGameObject
{
    friend class CScene;

protected:
    CBubbleParticle();
    CBubbleParticle(const CBubbleParticle& obj);
    virtual ~CBubbleParticle();

public:
    virtual bool Init() override;
    virtual CBubbleParticle* Clone() override;

private:
    CSharedPtr<class CParticleComponent> mParticle;
};

