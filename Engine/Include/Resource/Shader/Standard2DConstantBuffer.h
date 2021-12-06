#pragma once

#include "ConstantBufferBase.h"

class CStandard2DConstantBuffer :
    public CConstantBufferBase
{
public:
    CStandard2DConstantBuffer();
    CStandard2DConstantBuffer(const CStandard2DConstantBuffer& buffer);
    virtual ~CStandard2DConstantBuffer();

public:
    virtual bool Init();
    virtual void UpdateCBuffer();
    virtual CStandard2DConstantBuffer* Clone();

public:
    void SetAnimation2DEnable(bool bEnable)
    {
        mBufferData.AnimationEnable = bEnable ? 1 : 0;
    }

protected:
    Standard2DCBuffer mBufferData;
};

