#pragma once

#include "IMGUIWidget.h"

class CIMGUIMenuItem :
    public CIMGUIWidget
{
public:
    CIMGUIMenuItem();
    virtual ~CIMGUIMenuItem();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    void SetShortCut(const std::string& shortCut)
    {
        mShortCut = mShortCut;
    }

    void SetSelect(bool bSelect)
    {
        mbSelect = bSelect;
    }

    void SetEnable(bool bEnable)
    {
        mbEnable = bEnable;
    }

public:
    bool IsSelected()
    {
        return mbSelect;
    }

public:
    template <typename T>
    void SetClickCallBack(T* obj, void(T::* func)())
    {
        mCallBack = std::bind(func, obj);
    }

private:
    std::string mShortCut;
    bool mbSelect;
    bool mbEnable;
    std::function<void()> mCallBack;
};

