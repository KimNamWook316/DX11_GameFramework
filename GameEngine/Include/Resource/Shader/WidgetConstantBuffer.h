#pragma once

#include "ConstantBufferBase.h"

class CWidgetConstantBuffer :
    public CConstantBufferBase
{
public:
    CWidgetConstantBuffer();
    CWidgetConstantBuffer(const CWidgetConstantBuffer& buffer);
    virtual ~CWidgetConstantBuffer();

public:
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CWidgetConstantBuffer* Clone() override;
    
public:
    void SetTint(const Vector4& tint)
    {
        mBufferData.Tint = tint;
    }

    void SetWP(const Matrix& mat)
    {
        mBufferData.MatWP = mat;
    }

    void SetUseTextre(bool bUse)
    {
        mBufferData.UseTexture = bUse ? 1 : 0;
    }

    void SetOpacity(float opacity)
    {
        mBufferData.Opacity = opacity;
    }

protected:
    WidgetCBuffer mBufferData;
};

