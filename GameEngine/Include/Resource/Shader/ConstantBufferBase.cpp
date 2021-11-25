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
	// 리소스 매니저에 생성되어 있는 상수 버퍼를 자신도 들고 있게 한다.
	mBuffer = CResourceManager::GetInst()->FindConstantBuffer(name);
}
