#include "GlobalConstantBuffer.h"
#include "ConstantBuffer.h"

CGlobalConstantBuffer::CGlobalConstantBuffer()	:
	mBufferData{}
{
}

CGlobalConstantBuffer::CGlobalConstantBuffer(const CGlobalConstantBuffer& buffer)	:
	CConstantBufferBase(buffer)
{
	mBufferData = buffer.mBufferData;
}

CGlobalConstantBuffer::~CGlobalConstantBuffer()
{
}

bool CGlobalConstantBuffer::Init()
{
	setMyConstantBuffer("GlobalCBuffer");
	return true;
}

void CGlobalConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CGlobalConstantBuffer* CGlobalConstantBuffer::Clone()
{
	return new CGlobalConstantBuffer(*this);
}
