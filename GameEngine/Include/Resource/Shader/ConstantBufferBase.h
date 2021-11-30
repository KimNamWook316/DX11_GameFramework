#pragma once

#include "../../GameInfo.h"

// ��� ���۸� ��� �ִ� ��ü.
class CConstantBufferBase
{
public:
	CConstantBufferBase();
	CConstantBufferBase(const CConstantBufferBase& buffer);
	virtual ~CConstantBufferBase();

public:
	virtual bool Init() = 0;
	virtual void UpdateCBuffer() = 0;
	virtual CConstantBufferBase* Clone() = 0;

protected:
	void setMyConstantBuffer(const std::string& name);

protected:
	CSharedPtr<class CConstantBuffer> mBuffer;
};
