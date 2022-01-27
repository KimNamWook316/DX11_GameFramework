#include "ParticleConstantBuffer.h"
#include "ConstantBuffer.h"

CParticleConstantBuffer::CParticleConstantBuffer()	:
	mBufferData{}
{
	mBufferData.SpawnCountMax = 100;
	mBufferData.ColorMin = Vector4(1.f, 1.f, 1.f, 1.f);
	mBufferData.ColorMax = Vector4(1.f, 1.f, 1.f, 1.f);
	mBufferData.LifeTimeMin = 0.5f;
	mBufferData.LifeTimeMax = 1.f;
	mBufferData.ScaleMin = Vector3(30.f, 30.f, 1.f);
	mBufferData.ScaleMax = Vector3(30.f, 30.f, 1.f);
	mBufferData.SpeedMin = 1.f;
	mBufferData.SpeedMax = 3.f;
	mBufferData.StartMin = Vector3(-10.f, -10.f, 0.f);
	mBufferData.StartMin = Vector3(10.f, 10.f, 0.f);
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
