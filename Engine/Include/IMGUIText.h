#pragma once

#include "IMGUIWidget.h"

class CIMGUIText :
    public CIMGUIWidget
{
public:
    CIMGUIText();
    virtual ~CIMGUIText();

public:
    virtual bool Init() override;
    virtual void Render() override;

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

    void AddText(const char* text)
    {
        wchar_t wText[1024] = {};
        
        int length = MultiByteToWideChar(CP_ACP, 0, text, -1, 0, 0);
        MultiByteToWideChar(CP_ACP, 0, text, -1, wText, length);

        strcat_s(mText, text);
        lstrcat(mwText, wText);

        length = WideCharToMultiByte(CP_UTF8, 0, mwText, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_UTF8, 0, mwText, -1, mTextUTF8, length, 0, 0);
    }

protected:
    char        mText[1024];
    wchar_t     mwText[1024];
    char        mTextUTF8[1024];
};

