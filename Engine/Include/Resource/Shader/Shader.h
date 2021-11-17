#pragma once

#include "../../Ref.h"

class CShader : public CRef
{
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
	// Compute Shader����, ������ ���������ο� �����ϴ� Graphic ���̴����� �����ϱ� ����
	Shader_Type meType;
};