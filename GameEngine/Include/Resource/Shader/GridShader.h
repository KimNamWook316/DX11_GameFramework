#pragma once

#include "GraphicShader.h"

class CGridShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CGridShader();
    virtual ~CGridShader();

public:
    virtual bool Init() override;
};

