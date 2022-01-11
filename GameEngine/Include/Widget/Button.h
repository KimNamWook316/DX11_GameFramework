#pragma once

#include "Widget.h"
#include "../Resource/Texture/Texture.h"

struct ButtonStateInfo
{
    CSharedPtr<CTexture> Texture;
    Vector4 Tint;
    std::vector<AnimationFrameData> vecFrameData;

    ButtonStateInfo()
    {
        Tint = Vector4::White;
    }
};

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

public:
    void ButtonEnable(bool bEnable)
    {
        meCurState = bEnable ? eButtonState::Normal : eButtonState::Disabled;
    }

    void SetTexture(eButtonState state, CTexture* texture)
    {
        mInfo->Texture = texture;
    }

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
    eButtonState meCurState;
    ButtonStateInfo mInfo[(int)eButtonState::Max];
    std::function<void()> mClickCallBack;

    CSharedPtr<class CSound> mSound[(int)eButtonSoundState::Max];
    bool mbHoverSoundPlaying;
    bool mbClickSoundPlaying;
};

