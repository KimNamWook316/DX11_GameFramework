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
	bool LoadComputeShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
	void Excute(unsigned int x, unsigned int y, unsigned int z);

protected:
	ID3D11ComputeShader* mCS;
	ID3DBlob* mCSBlob;
};

