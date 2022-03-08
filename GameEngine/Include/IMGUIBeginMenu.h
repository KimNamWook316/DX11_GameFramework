#pragma once

#include "IMGUIWidget.h"

class CIMGUIBeginMenu :
    public CIMGUIWidget
{
public:
    CIMGUIBeginMenu();
    virtual ~CIMGUIBeginMenu();

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
    size_t mSize;
};

