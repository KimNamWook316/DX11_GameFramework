#pragma once

#include "IMGUIWidget.h"

class CIMGUITree :
    public CIMGUIWidget
{
    friend class CIMGUIWindow;

protected:
    CIMGUITree();
    virtual ~CIMGUITree();

public:
    void SetParent(CIMGUITree* parent)
    {
        mParent = parent;
    }

    bool IsRootNode()
    {
        return mParent == nullptr;
    }

    bool IsEndNode()
    {
        return mVecChild.size() == 0;
    }

public:
    CIMGUITree* GetNode(const std::string& name);
    CIMGUITree* GetNode(const int idx);

public:
    CIMGUITree* AddChild(const std::string& name, const float width = 100.f, const float height = 100.f);

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
		mVecWidget.push_back(widget);
        mVecRender.push_back(widget);
        return widget;
    }

public:
    virtual bool Init();
    virtual void Render();

protected:
    CIMGUITree* mParent;
    std::vector<CIMGUITree*> mVecChild;
    std::vector<CIMGUIWidget*> mVecWidget;
    std::vector<CIMGUIWidget*> mVecRender;
};

