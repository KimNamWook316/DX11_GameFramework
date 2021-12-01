#pragma once

#include "../../Ref.h"

class CRenderState :
    public CRef
{
    friend class CRenderManager;

protected:
    CRenderState();
    virtual ~CRenderState();

public:
    virtual void SetState() = 0;
    virtual void ResetState() = 0;

public:
    eRenderStateType GetRenderStateType() const
    {
        return meStateType;
    }

protected:
    // 각 ID3D11RenderState들이 상속받는 클래스.
    // 모든 RenderState 클래스들이 이 클래스를 상속받게 때문에 이 클래스의 포인터로
    ID3D11DeviceChild* mState;
    ID3D11DeviceChild* mPrevState;

    // 어떤 RenderState인지 판별하기 위한 enum
    eRenderStateType meStateType;
};

