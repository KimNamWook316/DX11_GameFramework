#pragma once

#include "IMGUIWidget.h"

class CIMGUIButton :
    public CIMGUIWidget
{
    friend class CIMGUIWindow;
    friend class CIMGUITree;

protected:
    CIMGUIButton();
    virtual ~CIMGUIButton();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    template <typename T>
    void SetClickCallBack(T* obj, void(T::* func)())
    {
        mClickCallBack = std::bind(func, obj);
    }

protected:
    std::function<void()> mClickCallBack;
    bool                  mbClick;
};

