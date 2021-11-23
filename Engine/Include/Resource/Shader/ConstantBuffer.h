#pragma once

#include "../../Ref.h"

class CConstantBuffer : public CRef
{
	friend class CShaderManager;

private:
	CConstantBuffer();
	~CConstantBuffer();

public:
	bool Init(const int& size, const int& reg, int constantBufferType);
	void UpdateBuffer(void* data);

private:
	ID3D11Buffer* mBuffer;
	int	mSize;
	int mRegister;
	int mConstantBufferShaderType;
};

