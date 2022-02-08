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
	// Compute Shader인지, 렌더링 파이프라인에 관여하는 Graphic 쉐이더인지 구분하기 위함
	eShaderType meType;
};