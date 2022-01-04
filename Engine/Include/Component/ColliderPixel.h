#pragma once

#include "ColliderComponent.h"

class CColliderPixel :
    public CColliderComponent 
{
    friend class CGameObject;

protected:
    CColliderPixel();
    CColliderPixel(const CColliderPixel& com);
    virtual ~CColliderPixel();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CColliderPixel* Clone();
    virtual void Save(FILE* fp) override;
    virtual void Load(FILE* fp) override;

    virtual bool DoCollide(CColliderComponent* Dest) override;
    virtual bool DoCollideMouse(const Vector2& mousePos) override;

public:
    PixelInfo GetInfo() const
    {
        return mInfo;
    }

public:
    void SetInfo(const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    void SetInfoFullPath(const TCHAR* fullPath);
    void SetInfoMultiByte(const char* fileName, const std::string& pathName = TEXTURE_PATH);
    void SetInfoMultibyteFullPath(const char* fullPath);
    void SetPixelCollisionType(ePixelCollisionType eType);
    void SetPixelColor(unsigned char r, unsigned char g, unsigned char b);
    void SetPixelAlpha(unsigned char a);

protected:
    PixelInfo mInfo;
    CSharedPtr<class CMesh> mPixelMesh;
    CSharedPtr<class CShader> mPixelShader;
};

