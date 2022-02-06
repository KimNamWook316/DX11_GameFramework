#include "DissolveConstantBuffer.h"
#include "ConstantBuffer.h"

CDissolveConstantBuffer::CDissolveConstantBuffer()	:
	mBufferData{}
{
	mBufferData.OutLineColor = Vector4(0.f, 0.f, 0.f, 0.f);
	mBufferData.OutFilter = 0.1f;
	mBufferData.CenterFilter = 0.07f;
	mBufferData.InFilter = 0.05f;
}

CDissolveConstantBuffer::CDissolveConstantBuffer(const CDissolveConstantBuffer& buffer) :
	CConstantBufferBase(buffer)
{
	mBufferData = buffer.mBufferData;
}

CDissolveConstantBuffer::~CDissolveConstantBuffer()
{
}

bool CDissolveConstantBuffer::Init()
{
	setMyConstantBuffer("DissolveCBuffer");
}

void CDissolveConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CDissolveConstantBuffer* CDissolveConstantBuffer::Clone()
{
	return new CDissolveConstantBuffer(*this);
}
