#pragma once

#include "Widget.h"
#include "../Resource/Shader/ProgressBarConstantBuffer.h"

class CProgressBar :
    public CWidget
{
    friend class CWidgetWindow;

protected:
    CProgressBar();
    CProgressBar(const CProgressBar& widget);
    virtual ~CProgressBar();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CProgressBar* Clone() override;

public:
    float GetPercent() const
    {
        return mPercent;
    }

public:
    // 0.f ~ 1.f
    void SetPercent(const float percent)
    {
        mPercent = percent;

        if (mPercent > 1.f)
        {
            mPercent = 1.f;
        }
        else if (mPercent < 0.f)
        {
            mPercent = 0.f;
        }

        mProgressBarCBuffer->SetPercent(percent);
    }

    void SetDir(eProgressBarDir dir)
    {
        meDir = dir;
        mProgressBarCBuffer->SetDir(dir);
    }

    virtual void Enable(bool bEnable)
    {
        CWidget::Enable(bEnable);
    }

    void SetTexture(CTexture* texture);
    bool SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
    void SetTextureTint(const Vector4& tint);
    void SetTextureTint(const float r, const float g, const float b, const float a);
    void AddFrameData(const Vector2& start, const Vector2& size);

public:
    template <typename T>
    void SetMouseHoverCallBack(T* obj, void(T::* func)())
    {
        mMouseHoverCallBack = std::bind(func, obj);
    }

    void SetMouseHoverCallBack(void(*func)())
    {
        mMouseHoverCallBack = std::bind(func);
    }

protected:
    //WidgetImageInfo mInfo;
    class CWidgetImageData* mImageData;
    std::function<void()> mMouseHoverCallBack;
    float mPercent;
    eProgressBarDir meDir;
    CProgressBarConstantBuffer* mProgressBarCBuffer;
    bool mbStartMouseHover;
};

