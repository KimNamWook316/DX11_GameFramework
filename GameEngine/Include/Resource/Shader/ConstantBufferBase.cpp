#include "ConstantBufferBase.h"
#include "ConstantBuffer.h"
#include "../ResourceManager.h"

CConstantBufferBase::CConstantBufferBase()
{
}

CConstantBufferBase::CConstantBufferBase(const CConstantBufferBase& buffer)
{
	mBuffer = buffer.mBuffer;
}

CConstantBufferBase::~CConstantBufferBase()
{
}

void CConstantBufferBase::setMyConstantBuffer(const std::string& name)
{
	// ���ҽ� �Ŵ����� �����Ǿ� �ִ� ��� ���۸� �ڽŵ� ��� �ְ� �Ѵ�.
	mBuffer = CResourceManager::GetInst()->FindConstantBuffer(name);
}
