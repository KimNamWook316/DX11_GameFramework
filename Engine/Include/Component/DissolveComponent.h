#pragma once

#include "ObjectComponent.h"
#include "../Resource/Shader/DissolveConstantBuffer.h"

class CDissolveComponent :
    public CObjectComponent
{
    friend class CGameObject;

public:
    CDissolveComponent();
    CDissolveComponent(CDissolveComponent& com);
    virtual ~CDissolveComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CDissolveComponent* Clone() override;

public:
    void StartDissolve();
    void SetShader();

public:
    void SetMaterial(class CMaterial* material);
    void SetInverse(bool bEnable);
    void SetFinishTime(const float time)
    {
        mFinishTime = time;
    }
    void SetDissolveTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    void SetOutLineColor(const Vector4& color);
    void SetOutLineColor(const float r, const float g, const float b, const float a);
    void SetInLineColor(const Vector4& color);
    void SetInLineColor(const float r, const float g, const float b, const float a);
    void SetCenterLineColor(const Vector4& color);
    void SetCenterLineColor(const float r, const float g, const float b, const float a);
    void SetInFilter(const float filter);
    void SetOutFilter(const float filter);
    void SetCenterFilter(const float filter);

public:
    template <typename T>
    void SetFinishCallBack(T* obj, void(T::* func)())
    {
        mFinishCallBack = std::bind(func, obj);
    }

private:
    CDissolveConstantBuffer* mCBuffer;
    std::function<void()> mFinishCallBack;
    float mFinishTime;
    CSharedPtr<class CMaterial> mMaterial;
    CSharedPtr<class CTexture> mDissolveTexture;
    bool mbStart;
    float mFilter;
};

