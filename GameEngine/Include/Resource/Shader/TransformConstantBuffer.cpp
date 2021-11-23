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
	// CConstantBuffer ��ü�� �����´�.
	setConstantBuffer("TransformBuffer");
	return true;
}

void CTransformConstantBuffer::UpdateCBuffer()
{
	mBufferData.MatWV = mBufferData.MatWorld * mBufferData.MatView;
	mBufferData.MatWVP = mBufferData.MatWV * mBufferData.MatProj;
	mBufferData.MatVP = mBufferData.MatView * mBufferData.MatProj;

	// ���̴� ������ ����Ϸ���, ����� ��ġ�ؾ� �Ѵ�.
	mBufferData.MatWorld.Transpose();
	mBufferData.MatView.Transpose();
	mBufferData.MatProj.Transpose();
	mBufferData.MatWV.Transpose();
	mBufferData.MatWVP.Transpose();
	mBufferData.MatVP.Transpose();

	// GPU�� ��� ���۸� ����Ѵ�.
	mBuffer->UpdateBuffer(&mBufferData);
}

CTransformConstantBuffer* CTransformConstantBuffer::Clone()
{
	return new CTransformConstantBuffer(*this);
}
