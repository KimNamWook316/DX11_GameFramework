#pragma once
#include "Shader.h"

// Compute Shader는, 렌더링 파이프라인에서 동떨어진, 독립된 쉐이더로, 그래픽스 렌더링과는 관계 없이 사용할 수 있다.

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

