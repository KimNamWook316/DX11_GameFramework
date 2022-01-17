#pragma once

#include "../Ref.h"
#include "Widget.h"

class CWidgetWindow :
    public CRef
{
    friend class CViewport;
    friend class CWidgetComponent;

protected:
    CWidgetWindow();
    CWidgetWindow(const CWidgetWindow& window);
    virtual ~CWidgetWindow();

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CWidgetWindow* Clone();

    virtual bool DoCollideMouse(const Vector2& mousePos);

public:
    class CWidgetComponent* GetWidgetComponent() const
    {
        return mOwnerComponent;
    }

    class CViewport* GetViewport() const
    {
        return mViewport;
    }

    int GetWidgetCount() const
    {
        return (int)mWidgetList.size();
    }

    Vector2 GetWindowPos() const
    {
        return mPos;
    }

    Vector2 GetWindowSize() const
    {
        return mSize;
    }

    int GetZOrder() const
    {
        return mZOrder;
    }

public:
    void SetZorder(const int zOrder)
    {
        mZOrder = zOrder;
    }

    void SetPos(const Vector2& pos)
    {
        mPos = pos;
    }

    void SetPos(const float x, const float y)
    {
        mPos = Vector2(x, y);
    }

    void SetSize(const Vector2& size)
    {
        mSize = size;
    }

    void SetSize(const float x, const float y)
    {
        mSize = Vector2(x, y);
    }

public:
    template <typename T>
    T* FindWidget(const std::string& name)
    {
        auto iter = mWidgetList.begin();
        auto iterEnd = mWidgetList.end();

        for (; iter != iterEnd; ++iter)
        {
            if (name == (*iter)->GetName())
            {
                return (T*)*(*iter);
            }
        }
        return nullptr;
    }

    template <typename T>
    T* CreateWidget(const std::string& name)
    {
		T* widget = new T;
		widget->SetName(name);
		widget->SetOwner(this);

		if (!widget->Init())
		{
			SAFE_RELEASE(widget);
			assert(false);
			return nullptr;
		}

		mWidgetList.push_back(widget);
		return widget;
    }

private:
    static bool sortWidget(CSharedPtr<CWidget> src, CSharedPtr<CWidget> dest);

protected:
    class CWidgetComponent* mOwnerComponent;
    class CViewport* mViewport;
    bool mbVisable;
    int mZOrder; // Window³¢¸®ÀÇ Zorder
    Vector2 mPos;
    Vector2 mSize;
    std::list<CSharedPtr<CWidget>> mWidgetList;
    bool mbStart;
};

