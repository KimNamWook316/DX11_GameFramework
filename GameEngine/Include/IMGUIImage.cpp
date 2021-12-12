#include "IMGUIImage.h"
#include "Resource/Texture/Texture.h"
#include "Resource/ResourceManager.h"

CIMGUIImage::CIMGUIImage() :
    mImageStart{},
    mImageEnd{},
    mTint{ 255.f, 255.f, 255.f, 255.f },
    mBorderColor{}
{
}

CIMGUIImage::~CIMGUIImage()
{
}

bool CIMGUIImage::Init()
{
    SetTexture("UIDefault", TEXT("DefaultUI.png"));

    SetImageStart(0.f, 0.f);

    return true;
}

void CIMGUIImage::Render()
{
    if (mTexture)
    {
        unsigned int width = mTexture->GetWidth();
        unsigned int height = mTexture->GetHeight();

        ImVec2 startUV = ImVec2(mImageStart.x / width, mImageStart.y / height);
        ImVec2 endUV = ImVec2(mImageEnd.x / width, mImageEnd.y / height);

        ImGui::Image(mTexture->GetResourceView(), mSize, startUV, endUV, mTint, mBorderColor);
    }
}

void CIMGUIImage::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    CResourceManager::GetInst()->LoadTexture(name, fileName, pathName);
    mTexture = CResourceManager::GetInst()->FindTexture(name);

    SetImageEnd((float)mTexture->GetWidth(), (float)mTexture->GetHeight());
}

void CIMGUIImage::SetTexture(const std::string& name)
{
    mTexture = CResourceManager::GetInst()->FindTexture(name);
    SetImageEnd((float)mTexture->GetWidth(), (float)mTexture->GetHeight());
}

void CIMGUIImage::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
    CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath);
    mTexture = CResourceManager::GetInst()->FindTexture(name);
    SetImageEnd((float)mTexture->GetWidth(), (float)mTexture->GetHeight());
}

void CIMGUIImage::SetTexture(CTexture* texture)
{
    mTexture = texture;
    SetImageEnd((float)mTexture->GetWidth(), (float)mTexture->GetHeight());
}
