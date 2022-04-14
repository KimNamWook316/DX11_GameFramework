#pragma once

#include "Widget.h"
#include "../Resource/Texture/Texture.h"
#include "WidgetImageData.h"

class CButton :
    public CWidget
{
    friend class CWidgetWindow;

protected:
    CButton();
    CButton(const CButton& button);
    virtual ~CButton();
    
public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;
    virtual CButton* Clone();

public:
    void ButtonEnable(bool bEnable)
    {
        meCurState = bEnable ? eButtonState::Normal : eButtonState::Disabled;
    }

    void SetTexture(eButtonState state, CTexture* texture)
    {
        mImageData[(int)state]->SetTexture(texture);
        SetUseTexture(true);
    }

public:
    bool SetTexture(eButtonState state, const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(eButtonState state, const std::string& name, const TCHAR* fullPath);
    void SetTextureTint(eButtonState state, const Vector4& tint);
    void SetTextureTint(eButtonState state, const float r, const float g, const float b, const float a);
    void AddFrameData(eButtonState state, const Vector2& start, const Vector2& size);
    void SetSound(eButtonSoundState state, const std::string& soundName);
    void SetSound(eButtonSoundState state, class CSound* sound);
    void SetSound(eButtonSoundState state, const std::string& channelGroupName,
        const std::string& soundName, const char* fileName,
        const std::string& pathName = SOUND_PATH);

public:
    template <typename T>
    void SetLClickCallBack(T* obj, void(T::* func)())
    {
        mLClickCallBack = std::bind(func, obj);
    }

    template <typename T>
    void SetLClickCallBack(void(T::* func)())
    {
        mLClickCallBack = std::bind(func);
    }

    template <typename T>
    void SetRClickCallBack(T* obj, void(T::* func)())
    {
        mLClickCallBack = std::bind(func, obj);
    }

    template <typename T>
    void SetRClickCallBack(void(T::* func)())
    {
        mLClickCallBack = std::bind(func);
    }

public:
    template <typename T>
    void SetHoverCallBack(T* obj, void(T::* func)())
    {
        mHoverCallBack = std::bind(func, obj);
    }

    template <typename T>
    void SetHoverCallBack(void(T::* func)())
    {
        mHoverCallBack = std::bind(func);
    }

protected:
    eButtonState meCurState;
    CWidgetImageData* mImageData[(int)eButtonState::Max];

    std::function<void()> mLClickCallBack;
    std::function<void()> mRClickCallBack;
    std::function<void()> mHoverCallBack;

    CSharedPtr<class CSound> mSound[(int)eButtonSoundState::Max];
    bool mbHoverSoundPlaying;
    bool mbClickSoundPlaying;
};

