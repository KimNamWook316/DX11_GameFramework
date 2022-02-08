#pragma once

#include "ComputeShader.h"

class CParticleUpdateShader :
    public CComputeShader
{
    friend class CShaderManager;

public:
    CParticleUpdateShader();
    CParticleUpdateShader(const CParticleUpdateShader& shader);
    virtual ~CParticleUpdateShader();

public:
    virtual bool Init() override;
    virtual CParticleUpdateShader* Clone();
};

