#pragma once

#include "GraphicShader.h"

class CPathFindTileShader :
    public CGraphicShader
{   
    friend class CShaderManager;

protected:
    CPathFindTileShader();
    virtual ~CPathFindTileShader();

public:
    virtual bool Init();

};

