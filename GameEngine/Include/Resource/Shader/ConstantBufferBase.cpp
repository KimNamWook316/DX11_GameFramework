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

void CConstantBufferBase::setConstantBuffer(const std::string& name)
{
	mBuffer = CResourceManager::GetInst()->FindConstantBuffer(name);
}
