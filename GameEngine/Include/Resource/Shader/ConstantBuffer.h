#pragma once

#include "../../Ref.h"

// 상수 버퍼 자체를 들고 있는 객체
// 상수 버퍼는 다양한 것들이 존재할 수 있다.
// 상수 버퍼를 Base Class에서 공유해서 사용할 것임.
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
	int meConstantBufferShaderType;
};

