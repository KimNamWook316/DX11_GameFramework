#include "MaterialConstantBuffer.h"
#include "ConstantBuffer.h"

CMaterialConstantBuffer::CMaterialConstantBuffer()	:
	mBufferData{}
{
}

CMaterialConstantBuffer::CMaterialConstantBuffer(const CMaterialConstantBuffer& buffer)
{
	mBufferData = buffer.mBufferData;
}

CMaterialConstantBuffer::~CMaterialConstantBuffer()
{
}

bool CMaterialConstantBuffer::Init()
{
	// 실제 생성된 버퍼를 가져옴 ( ShaderManager에서 생성 )
	setMyConstantBuffer("MaterialCbuffer");
	return true;
}

void CMaterialConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CMaterialConstantBuffer* CMaterialConstantBuffer::Clone()
{
	return new CMaterialConstantBuffer(*this);
}
