#include "Animation2DConstantBuffer.h"
#include "ConstantBuffer.h"

CAnimation2DConstantBuffer::CAnimation2DConstantBuffer()	:
	mBufferData{}
{
}

CAnimation2DConstantBuffer::CAnimation2DConstantBuffer(const CAnimation2DConstantBuffer& buffer)
{
	mBufferData = buffer.mBufferData;
}

CAnimation2DConstantBuffer::~CAnimation2DConstantBuffer()
{
}

bool CAnimation2DConstantBuffer::Init()
{
	setMyConstantBuffer("Animation2DCBuffer");
	return true;
}

void CAnimation2DConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CAnimation2DConstantBuffer* CAnimation2DConstantBuffer::Clone()
{
	return new CAnimation2DConstantBuffer(*this);
}
