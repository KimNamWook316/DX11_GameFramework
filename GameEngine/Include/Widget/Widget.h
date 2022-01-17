#pragma once

#include "../Ref.h"
#include "../Resource/Texture/Texture.h"

struct WidgetImageInfo
{
    CSharedPtr<CTexture> Texture;
    Vector4 Tint;
    std::vector<AnimationFrameData> vecFrameData;

    WidgetImageInfo()
    {
        Tint = Vector4::White;
    }
};

class CWidget :
    public CRef
{
    friend class CWidgetWindow;

protected:
    CWidget();
    CWidget(const CWidget& widget);
    virtual ~CWidget();

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void PostUpdate(float deltaTime);
    virtual void Render();
    virtual CWidget* Clone();

    virtual bool DoCollideMouse(const Vector2& mousePos);

public:
    class CWidgetWindow* GetOwner() const
    {
        return mOwner;
    }

    Vector2 GetWidgetPos() const
    {
        return mPos;
    }

    Vector2 GetWidgetSize() const
    {
        return mSize;
    }

    int GetZOrder() const
    {
        return mZOrder;
    }

    float GetAngle() const
    {
        return mAngle;
    }

public:
    void SetZorder(const int zOrder)
    {
        mZOrder = zOrder;
    }

    void SetPos(const Vector2& pos)
    {
        mPos = pos;
    }

    void SetPos(const float x, const float y)
    {
        mPos = Vector2(x, y);
    }

    virtual void SetSize(const Vector2& size)
    {
        mSize = size;
    }

    virtual void SetSize(const float x, const float y)
    {
        mSize = Vector2(x, y);
    }

    void SetOwner(class CWidgetWindow* owner)
    {
        mOwner = owner;
    }

    void SetAngle(const float angle)
    {
        mAngle = angle;
    }

    void SetShader(const std::string& name);
    void SetUseTexture(bool bUse);

protected:
    class CWidgetWindow* mOwner;
    int mZOrder;
    Vector2 mPos;
    Vector2 mRenderPos;
    Vector2 mSize;
    float mAngle;
    Vector4 mTint;
    bool mbStart;
    bool mbMouseHovered;
    bool mbCollisionMouseEnable;

    class CWidgetConstantBuffer* mCBuffer;
    CSharedPtr<class CMesh> mMesh;
    CSharedPtr<class CShader> mShader;
};

