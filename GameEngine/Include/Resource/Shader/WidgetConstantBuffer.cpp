#include "WidgetConstantBuffer.h"
#include "ConstantBuffer.h"

CWidgetConstantBuffer::CWidgetConstantBuffer()	:
	mBufferData{}
{
	mBufferData.Tint = Vector4(1.f,1.f,1.f,1.f);
}

CWidgetConstantBuffer::CWidgetConstantBuffer(const CWidgetConstantBuffer& buffer)
{
	mBufferData = buffer.mBufferData;
}

CWidgetConstantBuffer::~CWidgetConstantBuffer()
{
}

bool CWidgetConstantBuffer::Init()
{
	setMyConstantBuffer("WidgetCBuffer");
	return true;
}

void CWidgetConstantBuffer::UpdateCBuffer()
{
	mBuffer->UpdateBuffer(&mBufferData);
}

CWidgetConstantBuffer* CWidgetConstantBuffer::Clone()
{
	return new CWidgetConstantBuffer(*this);
}
