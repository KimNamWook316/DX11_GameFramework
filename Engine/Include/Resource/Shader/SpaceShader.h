#pragma once

#include "GraphicShader.h"

class CSpaceShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CSpaceShader();
    virtual ~CSpaceShader();

public:
    virtual bool Init();
};

