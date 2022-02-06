#pragma once

#include "GameObject\GameObject.h"

class CSmokeParticle :
    public CGameObject
{
    friend class CScene;

protected:
    CSmokeParticle();
    CSmokeParticle(const CSmokeParticle& obj);
    virtual ~CSmokeParticle();

public:
    virtual bool Init() override;
    virtual CSmokeParticle* Clone() override;

private:
    CSharedPtr<class CParticleComponent> mParticle;
};

