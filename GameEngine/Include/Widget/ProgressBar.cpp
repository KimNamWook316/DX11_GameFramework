#include "ProgressBar.h"
#include "WidgetWindow.h"
#include "../Scene/Viewport.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "WidgetImageData.h"

CProgressBar::CProgressBar()	:
	mImageData(nullptr),
	mMouseHoverCallBack(nullptr),
	mPercent(1.f),
	meDir(eProgressBarDir::RightToLeft),
	mProgressBarCBuffer(nullptr),
	mbStartMouseHover(false)
{
	SetTypeID<CProgressBar>();
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
	SAFE_DELETE(mImageData);
	SAFE_DELETE(mProgressBarCBuffer);
}

bool CProgressBar::Init()
{
	if (!CWidget::Init())
	{
		assert(false);
		return false;
	}

	if (mOwner->GetViewport())
	{
		mShader = mOwner->GetViewport()->GetScene()->GetResource()->FindShader("ProgressBarShader");
	}
	else
	{
		mShader = CResourceManager::GetInst()->FindShader("ProgressBarShader");
	}

	mImageData = new CWidgetImageData;
	mImageData->SetOwnerWidget(this);
	mImageData->Init();

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
	mImageData->Update(deltaTime);

	if (mbMouseHovered && !mbStartMouseHover)
	{
		mbStartMouseHover = true;
		if (mMouseHoverCallBack)
		{
			mMouseHoverCallBack();
		}
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
	mTint = mImageData->GetImageTint();
	mImageData->SetShaderData();
	mProgressBarCBuffer->UpdateCBuffer();
	CWidget::Render();
}

CProgressBar* CProgressBar::Clone()
{
	return new CProgressBar(*this);
}

void CProgressBar::SetTexture(CTexture* texture)
{
	mImageData->SetTexture(texture);
}

bool CProgressBar::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	mImageData->SetTexture(name, fileName, pathName);
	SetUseTexture(true);
	return true;
}

bool CProgressBar::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	mImageData->SetTextureFullPath(name, fullPath);
	SetUseTexture(true);
	return true;
}

void CProgressBar::SetTextureTint(const Vector4& tint)
{
	mImageData->SetTextureTint(tint);
}

void CProgressBar::SetTextureTint(const float r, const float g, const float b, const float a)
{
	mImageData->SetTextureTint(r,g,b,a);
}

void CProgressBar::AddFrameData(const Vector2& start, const Vector2& size)
{
	mImageData->AddFrameData(start, size);
}
