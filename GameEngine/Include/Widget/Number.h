#pragma once

#include "Widget.h"

class CNumber :
    public CWidget
{
    friend class CWidgetWindow;

protected:
    CNumber();
    CNumber(const CNumber& widget);
    virtual ~CNumber();

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CNumber* Clone();

public:
    void SetNumber(const int num)
    {
        mNumber = num;
    }

    void AddNumber(const int num)
    {
        mNumber += num;
    }

public:
	void SetTexture(CTexture* texture);
    bool SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
    bool SetTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);
    void SetTextureTint(const Vector4& tint);
    void SetTextureTint(const float r, const float g, const float b, const float a);
    void AddFrameData(const Vector2& start, const Vector2& size);
	void AddFrameDataFrameTexture(const int count);

protected:
    WidgetImageInfo mInfo;
	class CAnimation2DConstantBuffer* mAnimationCBuffer;
    int mNumber;
    std::vector<int> mVecNumber;
};

