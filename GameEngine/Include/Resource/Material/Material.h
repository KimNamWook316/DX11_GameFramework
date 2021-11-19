#pragma once

#include "../Shader/GraphicShader.h"

class CMaterial : public CRef
{
	friend class CMaterialManager;

protected:
	CMaterial();
	virtual ~CMaterial();

public:
	void SetShader(const std::string& shaderName);
	void Render();

protected:
	CSharedPtr<CGraphicShader> mShader;
};

