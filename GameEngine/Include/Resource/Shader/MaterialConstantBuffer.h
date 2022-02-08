#pragma once

#include "ConstantBufferBase.h"

class CMaterialConstantBuffer :
    public CConstantBufferBase
{
public:
    CMaterialConstantBuffer();
    CMaterialConstantBuffer(const CMaterialConstantBuffer& buffer);
    virtual ~CMaterialConstantBuffer();

public:
    virtual bool Init();
    virtual void UpdateCBuffer();
    virtual CMaterialConstantBuffer* Clone();

public:
    void SetBaseColor(const Vector4& baseColor)
    {
        mBufferData.BaseColor = baseColor;
    }

    void SetOpacity(const float opacity)
    {
        mBufferData.Opacity = opacity;
    }

    void SetDissolveEnable(bool bEnable)
    {
        mBufferData.DissolveEnable = bEnable ? 1 : 0;
    }

protected:
    MaterialCBuffer mBufferData;
};

