#pragma once

#include "../../Ref.h"

class CShader : public CRef
{
	friend class CShaderManager;

public:
	virtual bool Init() = 0;
	virtual void SetShader() = 0;

	eShaderType GetShaderType() const
	{
		return meType;
	}

protected:
	CShader();
	virtual ~CShader() = 0;

protected:
	// Compute Shader����, ������ ���������ο� �����ϴ� Graphic ���̴����� �����ϱ� ����
	eShaderType meType;
};