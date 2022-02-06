#pragma once

#include "GameObject\GameObject.h"

class CMuzzleParticle :
    public CGameObject
{
    friend class CScene;

protected:
    CMuzzleParticle();
    CMuzzleParticle(const CMuzzleParticle& obj);
    virtual ~CMuzzleParticle();

public:
    virtual bool Init() override;
    virtual CMuzzleParticle* Clone() override;

private:
    CSharedPtr<class CParticleComponent> mFlashParticle;
    CSharedPtr<class CParticleComponent> mFragmentParticle;
};

