#include "Image.h"
#include "WidgetWindow.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Viewport.h"
#include "../Input.h"
#include "WidgetImageData.h"

CImage::CImage()	:
	mClickCallBack(nullptr),
	mImageData(nullptr)
{
	SetTypeID<CImage>();
}

CImage::CImage(const CImage& button)	:
	CWidget(button)
{
	mClickCallBack = nullptr;
	mImageData = button.mImageData->Clone();
	mImageData->SetOwnerWidget(this);
}

CImage::~CImage()
{
	SAFE_DELETE(mImageData);
}

bool CImage::Init()
{
	if (!CWidget::Init())
	{
		assert(false);
		return false;
	}

	mImageData = new CWidgetImageData;
	mImageData->SetOwnerWidget(this);
	mImageData->Init();
	return true;
}

void CImage::Start()
{
	CWidget::Start();
}

void CImage::Update(float deltaTime)
{
	mImageData->Update(deltaTime);

	CWidget::Update(deltaTime);
}

void CImage::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);
}

void CImage::Render()
{
	mTint = mImageData->GetImageTint();
	// 텍스쳐, 애니메이션 처리
	mImageData->SetShaderData();

	// 실제 렌더
	CWidget::Render();
}

CImage* CImage::Clone()
{
	return new CImage(*this);
}

void CImage::SetTexture(CTexture* texture)
{
	mImageData->SetTexture(texture);
	SetUseTexture(true);
}

bool CImage::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	mImageData->SetTexture(name, fileName, pathName);
	SetUseTexture(true);
	return true;
}

bool CImage::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	mImageData->SetTextureFullPath(name, fullPath);
	SetUseTexture(true);
	return true;
}

bool CImage::SetTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	mImageData->SetTexture(name, vecFileName, pathName);
	SetUseTexture(true);
	return true;
}

bool CImage::SetTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
	mImageData->SetTextureFullPath(name, vecFullPath);
	SetUseTexture(true);
	return true;
}

void CImage::SetTextureTint(  const Vector4& tint)
{
	mImageData->SetTextureTint(tint);
}

void CImage::SetTextureTint(  const float r, const float g, const float b, const float a)
{
	mImageData->SetTextureTint(r,g,b,a);
}

void CImage::AddFrameData(const Vector2& start, const Vector2& size)
{
	mImageData->AddFrameData(start, size);
}

void CImage::AddFrameDataFrameTexture(const int count)
{
	mImageData->AddFrameDataFrameTexture(count);
}

void CImage::SetCurrentFrame(const int frame)
{
	mImageData->SetCurrentFrame(frame);
}

void CImage::SetAnimationPlayScale(const float scale)
{
	mImageData->SetAnimationPlayScale(scale);
}

int CImage::GetAnimationFrameCount()
{
	return (int)mImageData->GetAnimationFrameCount();
}
