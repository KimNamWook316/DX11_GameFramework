#include "Image.h"
#include "WidgetWindow.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Viewport.h"
#include "../Input.h"

CImage::CImage()	:
	mClickCallBack(nullptr)
{
}

CImage::CImage(const CImage& button)	:
	CWidget(button)
{
	mClickCallBack = nullptr;
}

CImage::~CImage()
{
}

bool CImage::Init()
{
	if (!CWidget::Init())
	{
		assert(false);
		return false;
	}
	return true;
}

void CImage::Start()
{
	CWidget::Start();
}

void CImage::Update(float deltaTime)
{
	CWidget::Update(deltaTime);
}

void CImage::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);
}

void CImage::Render()
{
	if (mInfo.Texture)
	{
		mInfo.Texture->SetShader(0, (int)eConstantBufferShaderTypeFlags::Pixel, 0);
	}
	mTint = mInfo.Tint;

	CWidget::Render();
}

CImage* CImage::Clone()
{
	return new CImage(*this);
}

bool CImage::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CSceneResource* sceneResource = mOwner->GetViewport()->GetScene()->GetResource();

	if (!sceneResource->LoadTexture(name, fileName, pathName))
	{
		assert(false);
		return false;
	}
	
	mInfo.Texture = sceneResource->FindTexture(name);

	SetUseTexture(true);
	return true;
}

bool CImage::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	CSceneResource* sceneResource = mOwner->GetViewport()->GetScene()->GetResource();

	if (!sceneResource->LoadTextureFullPath(name, fullPath))
	{
		assert(false);
		return false;
	}
	
	mInfo.Texture = sceneResource->FindTexture(name);
	
	SetUseTexture(true);
	return true;
}

void CImage::SetTextureTint(  const Vector4& tint)
{
	mInfo.Tint = tint;
}

void CImage::SetTextureTint(  const float r, const float g, const float b, const float a)
{
	mInfo.Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CImage::AddFrameData(const Vector2& start, const Vector2& size)
{
	AnimationFrameData data;
	data.Start = start;
	data.Size = size;

	mInfo.vecFrameData.push_back(data);
}
