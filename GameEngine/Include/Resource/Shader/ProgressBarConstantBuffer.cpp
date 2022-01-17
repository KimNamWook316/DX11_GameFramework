#include "ProgressBarConstantBuffer.h"
#include "ConstantBuffer.h"

CProgressBarConstantBuffer::CProgressBarConstantBuffer()	:
	mBufferData{}
{
}

CProgressBarConstantBuffer::CProgressBarConstantBuffer(const CProgressBarConstantBuffer& buffer) :
	CConstantBufferBase(buffer)
{
	mBufferData = buffer.mBufferData;
}

CProgressBarConstantBuffer::~CProgressBarConstantBuffer()
{
}

bool CProgressBarConstantBuffer::Init()
{
	setMyConstantBuffer("ProgressBarCBuffer");
	return true;
}

void CProgressBarConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CProgressBarConstantBuffer* CProgressBarConstantBuffer::Clone()
{
	return new CProgressBarConstantBuffer(*this);
}
