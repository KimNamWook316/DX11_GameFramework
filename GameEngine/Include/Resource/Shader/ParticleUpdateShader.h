#pragma once

#include "ComputeShader.h"

class CParticleUpdateShader :
    public CComputeShader
{
    friend class CShaderManager;

public:
    CParticleUpdateShader();
    virtual ~CParticleUpdateShader();

public:
    virtual bool Init() override;
};

