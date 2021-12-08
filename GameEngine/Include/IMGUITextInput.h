#pragma once

#include "IMGUIWidget.h"

enum class eImGuiTextInputType
{
    String,
    Int,
    Float
};

class CIMGUITextInput :
    public CIMGUIWidget
{
    friend class CIMGUIWindow;

protected:
    CIMGUITextInput();
    virtual ~CIMGUITextInput();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    int GetValueInt() const
    {
        return mValueInt;
    }

    float GetValueFloat() const
    {
        return mValueFloat;
    }

    const wchar_t* GetText() const
    {
        return mwText;
    }

    const char* GetTextUTF8() const
    {
        return mTextUTF8;
    }

    const char* GetTextMultiByte() const
    {
        return mText;
    }

public:
    void SetText(const char* text)
    {
        memset(mwText, 0, sizeof(wchar_t) * 1024);
        memset(mText, 0, sizeof(char) * 1024);
        memset(mTextUTF8, 0, sizeof(char) * 1024);

        strcpy_s(mText, text);

        // 받은 입력을 unicode로 저장 
        int length = MultiByteToWideChar(CP_ACP, 0, text, -1, 0, 0);
        MultiByteToWideChar(CP_ACP, 0, text, -1, mwText, length);

        // unicode -> utf8 ( multibyte )
        length = WideCharToMultiByte(CP_UTF8, 0, mwText, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_UTF8, 0, mwText, -1, mTextUTF8, length, 0, 0);
    }

    void SetHintText(const char* text)
    {
        memset(mwHintText, 0, sizeof(wchar_t) * 1024);
        memset(mHintText, 0, sizeof(char) * 1024);
        memset(mHintTextUTF8, 0, sizeof(char) * 1024);

        strcpy_s(mHintText, text);

        // 받은 입력을 unicode로 저장 
        int length = MultiByteToWideChar(CP_ACP, 0, text, -1, 0, 0);
        MultiByteToWideChar(CP_ACP, 0, text, -1, mwHintText, length);

        // unicode -> utf8 ( multibyte )
        length = WideCharToMultiByte(CP_UTF8, 0, mwHintText, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_UTF8, 0, mwHintText, -1, mHintTextUTF8, length, 0, 0);
    }

    void SetInt(const int value)
    {
        mValueInt = value;
    }

    void SetFloat(const float value)
    {
        mValueFloat = value;
    }

    void SetMultiLine(const bool bMultiLine)
    {
        mbMultiLine = bMultiLine;
    }
    
    void AddFlag(ImGuiInputTextFlags_ flag)
    {
        mFlag |= flag;
    }

    void ReadOnly(const bool bEnable)
    {
        if (bEnable)
        {
            mFlag |= ImGuiInputTextFlags_ReadOnly;
        }

        else if (mFlag & ImGuiInputTextFlags_ReadOnly)
        {
            mFlag ^= ImGuiInputTextFlags_ReadOnly;
        }
    }
    
    void EnablePassword(const bool bEnable)
    {
        if (bEnable)
        {
            mFlag |= ImGuiInputTextFlags_Password;
        }

        else if (mFlag & ImGuiInputTextFlags_Password)
        {
            mFlag ^= ImGuiInputTextFlags_Password;
        }
    }

    void SetTextType(eImGuiTextInputType eType)
    {
        meTextInputType = eType;
    }
    
public:
    template <typename T>
    void SetInputCallBack(T* obj, void(T::* func)())
    {
        mInputCallBack = std::bind(func, obj);
    }

protected:
    char        mText[1024];
    wchar_t     mwText[1024];
    char        mTextUTF8[1024];

    // 힌트 텍스트
    char        mHintText[1024];
    wchar_t     mwHintText[1024];
    char        mHintTextUTF8[1024];

    // 여러 줄 입력
    bool mbMultiLine;
    
    // 텍스트 인풋 플래그 ImGuiInputTextFlags_
    ImGuiInputTextFlags mFlag;

    eImGuiTextInputType meTextInputType;

    int mValueInt;
    float mValueFloat;
    std::function<void()> mInputCallBack;
};

