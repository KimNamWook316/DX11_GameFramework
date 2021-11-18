#pragma once

#include "../../Ref.h"

class CShader : public CRef
{
	friend class CShaderManager;

public:
	virtual bool Init() = 0;
	virtual void SetShader() = 0;

	Shader_Type GetShaderType() const
	{
		return meType;
	}

protected:
	CShader();
	virtual ~CShader() = 0;

protected:
	// Compute Shader인지, 렌더링 파이프라인에 관여하는 Graphic 쉐이더인지 구분하기 위함
	Shader_Type meType;
};