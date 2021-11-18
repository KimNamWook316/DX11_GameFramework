#pragma once
#include "Shader.h"

// Compute Shader��, ������ ���������ο��� ��������, ������ ���̴���, �׷��Ƚ� ���������� ���� ���� ����� �� �ִ�.

class CComputeShader : public CShader
{
	friend class CShaderManager;

protected:
	CComputeShader();
	virtual ~CComputeShader() = 0;

public:
	virtual bool Init() = 0;
	virtual void SetShader();
};

