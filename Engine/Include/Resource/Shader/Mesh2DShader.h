#pragma once

#include "GraphicShader.h"

class CMesh2DShader : public CGraphicShader
{
	friend class CShaderManager;

public:
	virtual bool Init();

protected:
	CMesh2DShader();
	virtual ~CMesh2DShader();
};

