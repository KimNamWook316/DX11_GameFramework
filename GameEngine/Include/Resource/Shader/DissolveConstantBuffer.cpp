#include "DissolveConstantBuffer.h"
#include "ConstantBuffer.h"

CDissolveConstantBuffer::CDissolveConstantBuffer()	:
	mBufferData{}
{
	mBufferData.OutLineColor = Vector4(1.f, 0.0f, 0.0f, 1.f);
	mBufferData.InLineColor = Vector4(0.f, 1.f, 0.f, 1.f);
	mBufferData.CenterLineColor = Vector4(0.f, 0.f, 1.f, 1.f);
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
	return true;
}

void CDissolveConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CDissolveConstantBuffer* CDissolveConstantBuffer::Clone()
{
	return new CDissolveConstantBuffer(*this);
}
