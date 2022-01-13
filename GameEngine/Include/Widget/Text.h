#pragma once

#include "Widget.h"

enum class eTextAlignH
{
    Left,
    Center,
    Right,
};

enum class eTextAlignV
{
    Top,
    Middle,
    Bottom,
};

class CText :
    public CWidget
{
    friend class CWidgetWindow;

protected:
    CText();
    CText(const CText& widget);
    virtual ~CText();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void Render() override;

public:
    bool CreateTextLayout();

public:
    const TCHAR* GetText() const
    {
        return mText;
    }

    int GetTextCount() const
    {
        return mTextCount;
    }

public:
    void SetText(const TCHAR* text)
    {
        int length = lstrlen(text);

        if (length + 1 > mTextCapacity)
        {
            mTextCapacity = length + 1;
            SAFE_DELETE_ARRAY(mText);
            mText = new TCHAR[mTextCapacity];
            memset(mText, 0, sizeof(TCHAR) * mTextCapacity);
        }
        lstrcpy(mText, text);
        mTextCount = length;

        CreateTextLayout();
    }

    void AddText(const TCHAR* text)
    {
        int length = mTextCount + lstrlen(text);

        if (length + 1 > mTextCapacity)
        {
            mTextCapacity = length + 1;
            
            TCHAR* newText = new TCHAR[mTextCapacity];

            memset(newText, 0, sizeof(TCHAR) * mTextCapacity);

            lstrcpy(newText, mText);
            
            SAFE_DELETE_ARRAY(mText);
            mText = newText;
        }
        lstrcat(mText, text);
        mTextCount += lstrlen(text);

        CreateTextLayout();
    }

    void PopBack()
    {
        if (mTextCount > 0)
        {
            --mTextCount;
            mText[mTextCount] = 0;
            CreateTextLayout();
        }
    }

    void Clear()
    {
        if (mTextCount > 0)
        {
            mTextCount = 0;
            memset(mText, 0, sizeof(TCHAR) * mTextCapacity);
            CreateTextLayout();
        }
    }

public:
    void SetFont(const std::string& name);
    void SetFontSize(const float size);
    void SetAlignH(eTextAlignH align);
    void SetAlignV(eTextAlignV align);
    void SetColor(const float r, const float g, const float b);
    void SetColor(const unsigned char r, const unsigned char g, const unsigned char b);
    void SetColor(const Vector4& color);
    void SetColor(unsigned int color);
    void SetAlphaEnable(bool bAlpha);
    void SetOpacity(const float opacity);

    void SetShadowEnable(bool bShadow);
    void SetShadowOffset(const Vector2& offset);
    void SetShadowOffset(const float x, const float y);
    void SetShadowColor(const float r, const float g, const float b);
    void SetShadowColor(const unsigned char r, const unsigned char g, const unsigned char b);
    void SetShadowColor(const Vector4& color);
    void SetShadowColor(unsigned int color);
    void SetShadowAlphaEnable(bool bAlpha);
    void SetShadowOpacity(const float opacity);

protected:
    TCHAR* mText;
    int mTextCount;
    int mTextCapacity;
    
protected:
    IDWriteTextFormat* mFont;
    IDWriteTextLayout* mLayout;
    ID2D1SolidColorBrush* mColorBrush;
    ID2D1RenderTarget* m2DTarget;
    std::string mFontKey;
    TCHAR* mFontName;
    float mFontSize;
    bool mbAlphaEnable;
    float mOpacity;
    Vector4 mColor;
    
    // Shadow
    bool mbShadowEnable;
    ID2D1SolidColorBrush* mShadowColorBrush;
    bool mbShadowAlphaEnable;
    float mShadowOpacity;
    Vector4 mShadowColor;
    Vector2 mShadowOffset;

    // Align
    eTextAlignH meAlignH;
    eTextAlignV meAlignV;
};

