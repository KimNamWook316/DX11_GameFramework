#pragma once
#include "GraphicShader.h"
class CSpaceRectShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CSpaceRectShader();
    virtual ~CSpaceRectShader();

public:
    virtual bool Init();
};

