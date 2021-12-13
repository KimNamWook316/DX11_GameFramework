#pragma once

#include "IMGUIWidget.h"

class CIMGUIImage :
    public CIMGUIWidget
{
    friend class CIMGUIWindow;
    friend class CIMGUITree;

protected:
    CIMGUIImage();
    virtual ~CIMGUIImage();

public:
    virtual bool Init() override;
    virtual void Render() override;

public:
    void SetImageStart(const float x, const float y)
    {
        mImageStart = ImVec2(x, y);
    }

    void SetImageEnd(const float x, const float y)
    {
        mImageEnd = ImVec2(x, y);
    }

    void SetTint(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
    {
        mTint = ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
    }

    void SetBorderColor(const unsigned char r, const unsigned char g, const unsigned char b)
    {
        mBorderColor = ImVec4(r / 255.f, g / 255.f, b / 255.f, 1.f);
	}
    
public:
    void SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    void SetTexture(const std::string& name);
    void SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
    void SetTexture(class CTexture* texture);

protected:
    CSharedPtr<class CTexture> mTexture;
    ImVec2 mImageStart;
    ImVec2 mImageEnd;
    ImVec4 mTint;
    ImVec4 mBorderColor;
};

