#include "Standard2DConstantBuffer.h"
#include "ConstantBuffer.h"

CStandard2DConstantBuffer::CStandard2DConstantBuffer()	:
	mBufferData{}
{
}

CStandard2DConstantBuffer::CStandard2DConstantBuffer(const CStandard2DConstantBuffer& buffer)
{
	mBufferData = buffer.mBufferData;
}

CStandard2DConstantBuffer::~CStandard2DConstantBuffer()
{
}

bool CStandard2DConstantBuffer::Init()
{
	setMyConstantBuffer("Standard2DCBuffer");

	return true;
}

void CStandard2DConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CStandard2DConstantBuffer* CStandard2DConstantBuffer::Clone()
{
	return new CStandard2DConstantBuffer(*this);
}
