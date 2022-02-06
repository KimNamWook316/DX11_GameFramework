#pragma once

#include "GameObject\GameObject.h"

class CRainParticle :
    public CGameObject
{
    friend class CScene;

protected:
    CRainParticle();
    CRainParticle(const CRainParticle& obj);
    virtual ~CRainParticle();

public:
    virtual bool Init() override;
    virtual CRainParticle* Clone() override;

private:
    CSharedPtr<class CParticleComponent> mParticle;
};

