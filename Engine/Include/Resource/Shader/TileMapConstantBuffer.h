#pragma once

#include "ConstantBufferBase.h"

class CTileMapConstantBuffer :
    public CConstantBufferBase
{
public:
    CTileMapConstantBuffer();
    CTileMapConstantBuffer(const CTileMapConstantBuffer& buffer);
    virtual ~CTileMapConstantBuffer();

public:
    virtual bool Init() override;
    virtual void UpdateCBuffer() override;
    virtual CTileMapConstantBuffer* Clone() override;

public:
    void SetImageSize(const Vector2& size)
    {
        mBufferData.ImageSize = size;
    }

    void SetTileSize(const Vector2& size)
    {
        mBufferData.Size = size;
    }

    void SetImageStart(const Vector2& start)
    {
        mBufferData.Start = start;
    }

    void SetImageEnd(const Vector2& end)
    {
        mBufferData.End = end;
    }

    void SetWVP(const Matrix& matWVP)
    {
        mBufferData.MatWVP = matWVP;
    }

protected:
    TileMapCBuffer mBufferData;
};

