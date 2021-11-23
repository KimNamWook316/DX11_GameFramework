#include "TransformConstantBuffer.h"
#include "ConstantBuffer.h"

CTransformConstantBuffer::CTransformConstantBuffer()	:
	mBufferData{}
{
}

CTransformConstantBuffer::CTransformConstantBuffer(const CTransformConstantBuffer& buf)	:
	CConstantBufferBase(buf)
{
	mBufferData = buf.mBufferData;
}

CTransformConstantBuffer::~CTransformConstantBuffer()
{
}

bool CTransformConstantBuffer::Init()
{	
	// CConstantBuffer 객체를 가져온다.
	setConstantBuffer("TransformBuffer");
	return true;
}

void CTransformConstantBuffer::UpdateCBuffer()
{
	mBufferData.MatWV = mBufferData.MatWorld * mBufferData.MatView;
	mBufferData.MatWVP = mBufferData.MatWV * mBufferData.MatProj;
	mBufferData.MatVP = mBufferData.MatView * mBufferData.MatProj;

	// 쉐이더 내에서 사용하려면, 행렬을 전치해야 한다.
	mBufferData.MatWorld.Transpose();
	mBufferData.MatView.Transpose();
	mBufferData.MatProj.Transpose();
	mBufferData.MatWV.Transpose();
	mBufferData.MatWVP.Transpose();
	mBufferData.MatVP.Transpose();

	// GPU에 상수 버퍼를 기록한다.
	mBuffer->UpdateBuffer(&mBufferData);
}

CTransformConstantBuffer* CTransformConstantBuffer::Clone()
{
	return new CTransformConstantBuffer(*this);
}
