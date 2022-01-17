#pragma once

#include "SceneComponent.h"
#include "../Widget/WidgetWindow.h"
#include "../Scene/Scene.h"

class CWidgetComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CWidgetComponent();
    CWidgetComponent(const CWidgetComponent& com);
    virtual ~CWidgetComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void Save(FILE* fp) override;
    virtual void Load(FILE* fp) override;
    virtual CWidgetComponent* Clone();

public:
    void SetWidgetSpace(eWidgetComponentSpace eSpace)
    {
        meSpace = eSpace;

        if (eWidgetComponentSpace::Screen == meSpace)
        {
            SetBoolInheritRotX(false);
            SetBoolInheritRotY(false);
            SetBoolInheritRotZ(false);
            // GameObject Layer보다 위에 출력되는 레이어
            SetLayerName("ScreenWidgetComponent");
        }
        else
        {
            // 월드 공간에 출력되는 경우
            SetBoolInheritRotX(true);
            SetBoolInheritRotY(true);
            SetBoolInheritRotZ(true);
            SetLayerName("Default");
        }
    }

    void SetWidgetWindow(class CWidgetWindow* window)
    {
        if (mWidgetWindow)
        {
            mWidgetWindow->mOwnerComponent = nullptr;
        }

        mWidgetWindow = window;
        mWidgetWindow->mOwnerComponent = this;
    }

public:
    eWidgetComponentSpace GetWidgetSpace() const
    {
        return meSpace;
    }

    CWidgetWindow* GetWidgetWindow() const
    {
        return mWidgetWindow;
    }

public:
    template <typename T>
    T* CreateWidgetWindow(const std::string& name)
    {
        T* window = new T;
        window->SetName(name);
        window->mOwnerComponent = this;
        window->mViewport = mScene->GetViewport();

        if (!window->Init())
        {
            SAFE_RELEASE(window);
            return nullptr;
        }
        mWidgetWindow = window;
        return window;
    }

protected:
    CSharedPtr<CWidgetWindow> mWidgetWindow;
    eWidgetComponentSpace meSpace;
};

