#pragma once
#include "GraphicShader.h"
class CSpaceRhombusShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CSpaceRhombusShader();
    virtual ~CSpaceRhombusShader();

public:
    virtual bool Init();
};

