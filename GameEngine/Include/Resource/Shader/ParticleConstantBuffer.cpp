#include "ParticleConstantBuffer.h"
#include "ConstantBuffer.h"

CParticleConstantBuffer::CParticleConstantBuffer()	:
	mBufferData{}
{
}

CParticleConstantBuffer::CParticleConstantBuffer(const CParticleConstantBuffer& buffer)	:
	CConstantBufferBase(buffer)
{
	mBufferData = buffer.mBufferData;
}

CParticleConstantBuffer::~CParticleConstantBuffer()
{
}

bool CParticleConstantBuffer::Init()
{
	setMyConstantBuffer("ParticleCBuffer");
	return true;
}

void CParticleConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CParticleConstantBuffer* CParticleConstantBuffer::Clone()
{
	return new CParticleConstantBuffer(*this);
}
