#pragma once

#include "IMGUIWidget.h"

class CIMGUIWidgetList :
    public CIMGUIWidget
{
    friend class CIMGUIWindow;
    friend class CIMGUITree;
	friend class CIMGUIChild;

protected:
    CIMGUIWidgetList();
    virtual ~CIMGUIWidgetList();

public:
    virtual bool Init() override;
    virtual void Render() override;

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
};

