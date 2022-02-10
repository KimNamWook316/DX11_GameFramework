#include "TileMapConstantBuffer.h"
#include "ConstantBuffer.h"

CTileMapConstantBuffer::CTileMapConstantBuffer()	:
	mBufferData{}
{
}

CTileMapConstantBuffer::CTileMapConstantBuffer(const CTileMapConstantBuffer& buffer)
{
	mBufferData = buffer.mBufferData;
}

CTileMapConstantBuffer::~CTileMapConstantBuffer()
{
}

bool CTileMapConstantBuffer::Init()
{
	setMyConstantBuffer("TileMapCBuffer");

	return true;
}

void CTileMapConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CTileMapConstantBuffer* CTileMapConstantBuffer::Clone()
{
	return new CTileMapConstantBuffer(*this);
}
