#pragma once

#include "Resource/Shader/GraphicShader.h"

class CD2ShadowShader :
    public CGraphicShader
{
    friend class CShaderManager;

protected:
    CD2ShadowShader();
    virtual ~CD2ShadowShader();

public:
    virtual bool Init();
};

