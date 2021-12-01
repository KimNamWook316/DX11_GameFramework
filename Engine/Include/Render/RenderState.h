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
    // �� ID3D11RenderState���� ��ӹ޴� Ŭ����.
    // ��� RenderState Ŭ�������� �� Ŭ������ ��ӹް� ������ �� Ŭ������ �����ͷ�
    ID3D11DeviceChild* mState;
    ID3D11DeviceChild* mPrevState;

    // � RenderState���� �Ǻ��ϱ� ���� enum
    eRenderStateType meStateType;
};

