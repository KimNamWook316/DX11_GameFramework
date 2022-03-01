#include "SpaceConstantBuffer.h"
#include "ConstantBuffer.h"

CSpaceConstantBuffer::CSpaceConstantBuffer()	:
	mBufferData{}
{
}

CSpaceConstantBuffer::CSpaceConstantBuffer(const CSpaceConstantBuffer& buffer)
{
	mBufferData = buffer.mBufferData;
}

CSpaceConstantBuffer::~CSpaceConstantBuffer()
{
}

bool CSpaceConstantBuffer::Init()
{
	setMyConstantBuffer("SpaceCBuffer");
	return true;
}

void CSpaceConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CSpaceConstantBuffer* CSpaceConstantBuffer::Clone()
{
	return new CSpaceConstantBuffer(*this);
}
