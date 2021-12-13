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
    float mOffsetX; // 윈도우 창의 왼쪽에섲부터 어느정도 위치까지 떨어트릴지
    float mSpacing; // 시작 위치로부터 얼마나 떨어질지
};

