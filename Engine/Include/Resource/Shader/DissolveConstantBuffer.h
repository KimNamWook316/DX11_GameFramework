#pragma once

#include "ConstantBufferBase.h"

class CDissolveConstantBuffer :
    public CConstantBufferBase
{
public:
    CDissolveConstantBuffer();
    CDissolveConstantBuffer(const CDissolveConstantBuffer& buffer);
    virtual ~CDissolveConstantBuffer();

public:
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CDissolveConstantBuffer* Clone() override;

protected:
    DissolveCBuffer mBufferData;

public:
    void SetInColor(const Vector4& color)
    {
        mBufferData.InLineColor = color;
    }

    void SetInColor(const float r, const float g, const float b, const float a)
    {
        SetInColor(Vector4(r, g, b, a));
    }

    void SetOutColor(const Vector4& color)
    {
        mBufferData.OutLineColor = color;
    }

    void SetOutColor(const float r, const float g, const float b, const float a)
    {
        SetOutColor(Vector4(r, g, b, a));
    }

    void SetCenterColor(const Vector4& color)
    {
        mBufferData.CenterLineColor = color;
    }

    void SetCenterColor(const float r, const float g, const float b, const float a)
    {
        SetCenterColor(Vector4(r, g, b, a));
    }

    void SetFilter(float filter)
    {
        mBufferData.Filter = filter;
    }

    void SetInverse(bool inverse)
    {
        mBufferData.Inverse = inverse ? 1 : 0;
    }

    void SetInFilter(const float filter)
    {
        mBufferData.InFilter = filter;
    }

    void SetOutFilter(const float filter)
    {
        mBufferData.OutFilter = filter;
    }

    void SetCenterFilter(const float filter)
    {
        mBufferData.CenterFilter = filter;
    }
};

