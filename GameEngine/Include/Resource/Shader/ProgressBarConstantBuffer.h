#pragma once

#include "ConstantBufferBase.h"

class CProgressBarConstantBuffer :
    public CConstantBufferBase
{
public:
    CProgressBarConstantBuffer();
    CProgressBarConstantBuffer(const CProgressBarConstantBuffer& buffer);
    virtual ~CProgressBarConstantBuffer();

public:
	virtual bool Init() override;
	virtual void UpdateCBuffer() override;
	virtual CProgressBarConstantBuffer* Clone() override;

public:
    void SetPercent(const float percent)
    {
        mBufferData.Percent = percent;
    }

    void SetDir(eProgressBarDir dir)
    {
        mBufferData.Dir = (int)dir;
    }

private:
    ProgressBarCBuffer mBufferData;
};

