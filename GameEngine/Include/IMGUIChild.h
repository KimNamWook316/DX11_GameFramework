#pragma once

#include "IMGUIWidget.h"

class CIMGUIChild :
    public CIMGUIWidget
{
public:
    CIMGUIChild();
    virtual ~CIMGUIChild();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    void EnableBorder(bool bEnable)
    {
        mbBorder = bEnable;
    }

public:
    template <typename T>
    T* AddWidget(const std::string& name, const float width = 100.f, const float height = 100.f)
    {
		T* widget = new T;

		widget->SetName(name);
		widget->SetOwner(mOwner);
		widget->SetSize(width, height);

		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			assert(false);
			return nullptr;
		}
        mVecChild.push_back(widget);
        return widget;
    }

protected:
    std::vector<class CIMGUIWidget*> mVecChild;
    bool mbBorder;
};

