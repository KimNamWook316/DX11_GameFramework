#include "WidgetImageData.h"	
#include "Widget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"

CWidgetImageData::CWidgetImageData()	:
	mTint(Vector4::White),
	mAnimationCBuffer(nullptr),
	mbUseAnimation(false),
	mOwnerWidget(nullptr),
	mFrame(0),
	mTime(0.f),
	mFrameTime(0.f),
	mPlayTime(1.f),
	mPlayScale(1.f),
	mbIsLoop(false),
	mbIsReverse(false),
	mbPlay(true)
{
}

CWidgetImageData::CWidgetImageData(const CWidgetImageData& data)
{
	*this = data;
	mOwnerWidget = nullptr;
}

CWidgetImageData::~CWidgetImageData()
{
}

bool CWidgetImageData::Init()
{
	if (mOwnerWidget->GetOwner()->GetViewport())
	{
		mAnimationCBuffer = mOwnerWidget->GetOwner()->GetViewport()->GetScene()->GetResource()->GetAnimation2DCBuffer();
	}
	else
	{
		mAnimationCBuffer = CResourceManager::GetInst()->GetAnimation2DCBuffer();
	}
	return true;
}

void CWidgetImageData::Start()
{
}

void CWidgetImageData::Update(float deltaTime)
{
	if (!mbUseAnimation || !mbPlay || !mTexture)
	{
		return;
	}

	mTime += deltaTime * mPlayScale;
	
	bool bIsAnimEnd = false;

	if (mTime >= mFrameTime)
	{
		mTime -= mFrameTime;

		if (mbIsReverse)
		{
			--mFrame;

			if (mFrame < 0)
			{
				bIsAnimEnd = true;
			}
		}
		else
		{
			++mFrame;

			if (mFrame == mVecFrameData.size())
			{
				bIsAnimEnd = true;
			}
		}
	}
	
	if (bIsAnimEnd)
	{
		if (mbIsLoop)
		{
			if (mbIsReverse)
			{
				mFrame = mVecFrameData.size() - 1;
			}
			else
			{
				mFrame = 0;
			}
		}
		else
		{
			if (mbIsReverse)
			{
				mFrame = mVecFrameData.size() - 1;
			}
			else
			{
				mFrame = 0;
			}
		}
	}
}

void CWidgetImageData::SetShaderData()
{
	if (!mTexture)
	{
		return;
	}

	if (mbUseAnimation && mbPlay)
	{
		// 애니메이션 사용하는 경우, 쉐이더에서 애니메이션 사용하도록
		CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(true);
		CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();

		switch (mTexture->GetImageType())
		{
		case eImageType::Atlas:
		{
			// Animation UV, 상수 버퍼 넘기기
			Vector2 startUV, endUV;
			Vector2 start = mVecFrameData[mFrame].Start;
			Vector2 frameSize = mVecFrameData[mFrame].Size;

			startUV = start / Vector2((float)mTexture->GetWidth(), (float)mTexture->GetHeight());
			endUV = (start + frameSize) / Vector2((float)mTexture->GetWidth(), (float)mTexture->GetHeight());

			mAnimationCBuffer->SetStartUV(startUV);
			mAnimationCBuffer->SetEndUV(endUV);
			mTexture->SetShader(0, (int)eConstantBufferShaderTypeFlags::Pixel, 0);
		}
			break;
		case eImageType::Frame:
		{
			mAnimationCBuffer->SetStartUV(Vector2(0.f, 0.f));
			mAnimationCBuffer->SetEndUV(Vector2(1.f, 1.f));
			mTexture->SetShader(0, (int)eConstantBufferShaderTypeFlags::Pixel, mFrame);
		}
			break;
		default:
			assert(false);
			return;
		}

		mAnimationCBuffer->SetAnimation2DType(mTexture->GetImageType());
		mAnimationCBuffer->UpdateCBuffer();
	}
	else
	{
		mTexture->SetShader(0, (int)eConstantBufferShaderTypeFlags::Pixel, 0);
		CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(false);
		CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();
	}
}

void CWidgetImageData::SetTexture(CTexture* texture)
{
	mTexture = texture;
}

bool CWidgetImageData::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CSceneResource* sceneResource = mOwnerWidget->GetOwner()->GetViewport()->GetScene()->GetResource();

	if (!sceneResource->LoadTexture(name, fileName, pathName))
	{
		assert(false);
		return false;
	}
	
	mTexture = sceneResource->FindTexture(name);
	return true;
}

bool CWidgetImageData::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	CSceneResource* sceneResource = mOwnerWidget->GetOwner()->GetViewport()->GetScene()->GetResource();
 if (!sceneResource->LoadTextureFullPath(name, fullPath))
	{
		assert(false);
		return false;
	}
	
	mTexture = sceneResource->FindTexture(name);
	return true;
}

bool CWidgetImageData::SetTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	if (mOwnerWidget->GetOwner()->GetViewport())
	{
		if (!mOwnerWidget->GetOwner()->GetViewport()->GetScene()->GetResource()->LoadTexture(name, vecFileName, pathName))
		{
			assert(false);
			return false;
		}
		mTexture = mOwnerWidget->GetOwner()->GetViewport()->GetScene()->GetResource()->FindTexture(name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName);
		mTexture = CResourceManager::GetInst()->FindTexture(name);
	}
	return true;
}

bool CWidgetImageData::SetTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
	if (mOwnerWidget->GetOwner()->GetViewport())
	{
		if (!mOwnerWidget->GetOwner()->GetViewport()->GetScene()->GetResource()->LoadTexture(name, vecFullPath))
		{
			assert(false);
			return false;
		}
		mTexture = mOwnerWidget->GetOwner()->GetViewport()->GetScene()->GetResource()->FindTexture(name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(name, vecFullPath);
		mTexture = CResourceManager::GetInst()->FindTexture(name);
	}
}

void CWidgetImageData::SetTextureTint(const Vector4& tint)
{
	mTint = tint;
}

void CWidgetImageData::SetTextureTint(const float r, const float g, const float b, const float a)
{
	mTint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CWidgetImageData::AddFrameData(const Vector2& start, const Vector2& size)
{
	AnimationFrameData data;
	data.Start = start;
	data.Size = size;

	mVecFrameData.push_back(data);
	
	mFrameTime = mPlayTime / mVecFrameData.size();
	SetUseAnimation(true);
}
