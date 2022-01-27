#pragma once

#include "GraphicShader.h"

class CParticleRenderShader :
    public CGraphicShader
{
    friend class CShaderManager;

public:
    CParticleRenderShader();
    virtual ~CParticleRenderShader();

public:
    virtual bool Init() override;
};

