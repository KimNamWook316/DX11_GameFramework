#include "ProgressBar.h"
#include "WidgetWindow.h"
#include "../Scene/Viewport.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"

CProgressBar::CProgressBar()	:
	mInfo{},
	mMouseHoverCallBack(nullptr),
	mPercent(1.f),
	meDir(eProgressBarDir::RightToLeft),
	mProgressBarCBuffer(nullptr),
	mbStartMouseHover(false)
{
}

CProgressBar::CProgressBar(const CProgressBar& widget)	:
	CWidget(widget)
{
	mbStartMouseHover = false;
	mMouseHoverCallBack = nullptr;
	mPercent = widget.mPercent;
	meDir = widget.meDir;
	mProgressBarCBuffer = new CProgressBarConstantBuffer;
	mProgressBarCBuffer->Init();
	mProgressBarCBuffer->SetDir(meDir);
	mProgressBarCBuffer->SetPercent(mPercent);
}

CProgressBar::~CProgressBar()
{
	SAFE_DELETE(mProgressBarCBuffer);
}

bool CProgressBar::Init()
{
	if (!CWidget::Init())
	{
		assert(false);
		return false;
	}

	mShader = mOwner->GetViewport()->GetScene()->GetResource()->FindShader("ProgressBarShader");
	mProgressBarCBuffer = new CProgressBarConstantBuffer;
	mProgressBarCBuffer->Init();
	mProgressBarCBuffer->SetDir(meDir);
	mProgressBarCBuffer->SetPercent(mPercent);
	return true;
}

void CProgressBar::Start()
{
	CWidget::Start();
}

void CProgressBar::Update(float deltaTime)
{
	CWidget::Update(deltaTime);

	if (mbMouseHovered && !mbStartMouseHover)
	{
		mbStartMouseHover = true;
		mMouseHoverCallBack();
	}
	else if (!mbMouseHovered && mbStartMouseHover)
	{
		mbStartMouseHover = false;
	}
}

void CProgressBar::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);
}

void CProgressBar::Render()
{
	if (mInfo.Texture)
	{
		mInfo.Texture->SetShader(0, (int)eConstantBufferShaderTypeFlags::Pixel, 0);
	}
	mTint = mInfo.Tint;
	mProgressBarCBuffer->UpdateCBuffer();
	CWidget::Render();
}

CProgressBar* CProgressBar::Clone()
{
	return new CProgressBar(*this);
}

bool CProgressBar::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CSceneResource* resource = mOwner->GetViewport()->GetScene()->GetResource();
	
	if (!resource->LoadTexture(name, fileName, pathName))
	{
		assert(false);
		return false;
	}

	mInfo.Texture = resource->FindTexture(name);
	SetUseTexture(true);
	return true;
}

bool CProgressBar::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	CSceneResource* resource = mOwner->GetViewport()->GetScene()->GetResource();
	
	if (!resource->LoadTextureFullPath(name, fullPath))
	{
		assert(false);
		return false;
	}

	mInfo.Texture = resource->FindTexture(name);
	SetUseTexture(true);
	return true;
}

void CProgressBar::SetTextureTint(const Vector4& tint)
{
	mInfo.Tint = tint;
}

void CProgressBar::SetTextureTint(const float r, const float g, const float b, const float a)
{
	mInfo.Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CProgressBar::AddFrameData(const Vector2& start, const Vector2& size)
{
	AnimationFrameData data;
	data.Start = start;
	data.Size = size;
	mInfo.vecFrameData.push_back(data);
}
