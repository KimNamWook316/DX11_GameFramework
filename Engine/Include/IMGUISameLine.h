#pragma once

#include "IMGUIWidget.h"

class CIMGUISameLine :
    public CIMGUIWidget
{
    friend class CIMGUIWindow;
    friend class CIMGUITree;

protected:
    CIMGUISameLine();
    virtual ~CIMGUISameLine();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    void SetOffsetX(const float offsetX)
    {
        mOffsetX = offsetX;
    }

    void SetSpacing(const float spacing)
    {
        mSpacing = spacing;
    }

protected:
    float mOffsetX; // ������ â�� ���ʿ��A���� ������� ��ġ���� ����Ʈ����
    float mSpacing; // ���� ��ġ�κ��� �󸶳� ��������
};

