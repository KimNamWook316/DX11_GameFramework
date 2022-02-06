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
		const int reg, const bool bDynamic, const int structuredBufferShaderTypeFlag = (int)eBufferShaderTypeFlags::Compute);
	void UpdateBuffer(void* data, int count);
	void SetShader();
	void SetShader(const int reg, const int structuredBufferShaderTypeFlag);
	void ResetShader();
	void ResetShader(const int reg, const int structuredBufferShaderTypeFlag);
	void CopyData(void* data);
	void CopyResource(CStructuredBuffer* buffer);
	CStructuredBuffer* Clone();

public:
	ID3D11Buffer* GetBuffer() const
	{
		return mBuffer;
	}

	const std::string& GetName() const
	{
		return mName;
	}

private:
	std::string mName;
	D3D11_BUFFER_DESC mDesc;
	ID3D11Buffer* mBuffer;

	ID3D11ShaderResourceView* mSRV;		// Cpu -> Gpu 로 보낼 때 이 뷰를 통함 (dynamic buffer일 때)
	ID3D11UnorderedAccessView* mUAV;	// Gpu -> Cpu 로 받을 때 이 뷰를 통함 (!dynamic)

	unsigned int mSize;
	unsigned int mCount;
	int mStructuredBufferShaderType;
	int mRegister;
	bool mbDynamic;
};

