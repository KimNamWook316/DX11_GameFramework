#pragma once

#include "Widget.h"

class CImage :
    public CWidget
{
    friend class CWidgetWindow;

protected:
    CImage();
    CImage(const CImage& button);
    virtual ~CImage();
    
public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CImage* Clone();

public:
    virtual void Enable(bool bEnable)
    {
        CWidget::Enable(bEnable);
    }

    void SetTexture(CTexture* texture);

public:
    bool SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
    bool SetTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);
    void SetTextureTint(const Vector4& tint);
    void SetTextureTint(const float r, const float g, const float b, const float a);
    void AddFrameData(const Vector2& start, const Vector2& size);
	void AddFrameDataFrameTexture(const int count);
	void SetCurrentFrame(const int frame);
    void SetAnimationPlayScale(const float scale);

public:
    int GetAnimationFrameCount();

public:
    template <typename T>
    void SetClickCallBack(T* obj, void(T::* func)())
    {
        mClickCallBack = std::bind(func, obj);
    }

    template <typename T>
    void SetClickCallBack(void(T::* func)())
    {
        mClickCallBack = std::bind(func);
    }

protected:
    class CWidgetImageData* mImageData;
    std::function<void()> mClickCallBack;
};

