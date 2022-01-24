#pragma once

#include "../../GameInfo.h"

class CStructuredBuffer
{
public:
	CStructuredBuffer();
	CStructuredBuffer(const CStructuredBuffer& buffer);
	virtual ~CStructuredBuffer();

public:
	bool Init(const std::string& name, const unsigned int size, const unsigned int count,
		const int reg, const bool bDynamic, const int structuredBufferShaderTypeFlag);
	void UpdateBuffer(void* data, int count);
	void SetShader();
	void SetShader(const int reg, const int structuredBufferShaderTypeFlag);
	void ResetShader();
	void ResetShader(const int reg, const int structuredBufferShaderTypeFlag);
	void CopyData(void* data);
	void CopyResource(CStructuredBuffer* buffer);
	CStructuredBuffer* Clone();

private:
	std::string mName;
	D3D11_BUFFER_DESC mDesc;
	ID3D11Buffer* mBuffer;

	ID3D11ShaderResourceView* mSRV;
	ID3D11UnorderedAccessView* mUAV;

	unsigned int mSize;
	unsigned int mCount;
	int mStructuredBufferShaderType;
	int mRegister;
	bool mbDynamic;
};

