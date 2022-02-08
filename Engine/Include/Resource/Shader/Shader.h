#pragma once

#include "../../Ref.h"

class CShader : public CRef
{
	friend class CShaderManager;

protected:
	CShader();
	CShader(const CShader& shader);
	virtual ~CShader() = 0;

public:
	virtual bool Init() = 0;
	virtual void SetShader() = 0;
	virtual CShader* Clone() = 0;

	eShaderType GetShaderType() const
	{
		return meType;
	}

protected:
	// Compute Shader����, ������ ���������ο� �����ϴ� Graphic ���̴����� �����ϱ� ����
	eShaderType meType;
};