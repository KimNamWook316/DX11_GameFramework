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
	// ���� ������ ���۸� ������ ( ShaderManager���� ���� )
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
