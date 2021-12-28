#include "ColliderConstantBuffer.h"
#include "ConstantBuffer.h"

CColliderConstantBuffer::CColliderConstantBuffer()	:
	mBufferData{}
{
}

CColliderConstantBuffer::CColliderConstantBuffer(const CColliderConstantBuffer& buffer)
{
	mBufferData = buffer.mBufferData;
}

CColliderConstantBuffer::~CColliderConstantBuffer()
{
}

bool CColliderConstantBuffer::Init()
{
	setMyConstantBuffer("ColliderCBuffer");
	return true;
}

void CColliderConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CColliderConstantBuffer* CColliderConstantBuffer::Clone()
{
	return new CColliderConstantBuffer(*this);
}
