#include "Number.h"
#include "WidgetWindow.h"
#include "../Scene/Viewport.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"
#include "WidgetImageData.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"

CNumber::CNumber()	:
	mNumber(0),
	mAnimationCBuffer(nullptr)
{
	SetTypeID<CNumber>();
}

CNumber::CNumber(const CNumber& widget)
{
	mNumber = widget.mNumber;
	mAnimationCBuffer = widget.mAnimationCBuffer;
}

CNumber::~CNumber()
{
}

bool CNumber::Init()
{
	if (!CWidget::Init())
	{
		assert(false);
		return false;
	}

	if (mOwner->GetViewport())
	{
		mShader = mOwner->GetViewport()->GetScene()->GetResource()->FindShader("NumberShader");
		mAnimationCBuffer = mOwner->GetViewport()->GetScene()->GetResource()->GetAnimation2DCBuffer();
	}
	else
	{
		mShader = CResourceManager::GetInst()->FindShader("NumberShader");
		mAnimationCBuffer = CResourceManager::GetInst()->GetAnimation2DCBuffer();
	}

	return true;
}

void CNumber::Start()
{
	CWidget::Start();
}

void CNumber::Update(float deltaTime)
{
	CWidget::Update(deltaTime);
}

void CNumber::PostUpdate(float deltaTime)
{
	CWidget::PostUpdate(deltaTime);
}

void CNumber::Render()
{
	// 현재 숫자를 분리한다.
	int number = mNumber;

	mVecNumber.clear();

	if (number == 0)
	{
		mVecNumber.push_back(0);
	}
	else
	{
		std::stack<int> stackNumber;

		while (number > 0)
		{
			stackNumber.push(number % 10);
			number /= 10;
		}

		while (!stackNumber.empty())
		{
			mVecNumber.push_back(stackNumber.top());
			stackNumber.pop();
		}
	}

	size_t count = mVecNumber.size();

	for (size_t i = 0; i < count; ++i)
	{
		if (mInfo.Texture)
		{
			int frame = 0;
			switch (mInfo.Texture->GetImageType())
			{
			case eImageType::Atlas:
			{
				// Animation UV, 상수 버퍼 넘기기
				Vector2 startUV, endUV;
				Vector2 start = mInfo.vecFrameData[frame].Start;
				Vector2 frameSize = mInfo.vecFrameData[frame].Size;

				startUV = start / Vector2((float)mInfo.Texture->GetWidth(), (float)mInfo.Texture->GetHeight());
				endUV = (start + frameSize) / Vector2((float)mInfo.Texture->GetWidth(), (float)mInfo.Texture->GetHeight());

				mAnimationCBuffer->SetStartUV(startUV);
				mAnimationCBuffer->SetEndUV(endUV);
				mAnimationCBuffer->UpdateCBuffer();
			}
				break;
			case eImageType::Frame:
			{
				frame = mVecNumber[i];

				mAnimationCBuffer->SetStartUV(Vector2(0.f, 0.f));
				mAnimationCBuffer->SetEndUV(Vector2(1.f, 1.f));
				mAnimationCBuffer->UpdateCBuffer();
			}
				break;
			case eImageType::Array:
				break;
			default:
				assert(false);
				break;
			}
			mInfo.Texture->SetShader(0, (int)eConstantBufferShaderTypeFlags::Pixel, frame);
		}
		mTint = mInfo.Tint;
		mRenderPos.x += (mSize.x * (float)i);
		CWidget::Render();
	}
}

CNumber* CNumber::Clone()
{
	return new CNumber(*this);
}

void CNumber::SetTexture(CTexture* texture)
{
	mInfo.Texture = texture;
	SetUseTexture(true);
}

bool CNumber::SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (mOwner->GetViewport())
	{
		if (!mOwner->GetViewport()->GetScene()->GetResource()->LoadTexture(name, fileName, pathName))
		{
			assert(false);
			return false;
		}
		mInfo.Texture = mOwner->GetViewport()->GetScene()->GetResource()->FindTexture(name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(name, fileName, pathName);
		mInfo.Texture = CResourceManager::GetInst()->FindTexture(name);
	}
	SetUseTexture(true);
	return true;
}

bool CNumber::SetTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	if (mOwner->GetViewport())
	{
		if (!mOwner->GetViewport()->GetScene()->GetResource()->LoadTexture(name, fullPath))
		{
			assert(false);
			return false;
		}
		mInfo.Texture = mOwner->GetViewport()->GetScene()->GetResource()->FindTexture(name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(name, fullPath);
		mInfo.Texture = CResourceManager::GetInst()->FindTexture(name);
	}
	SetUseTexture(true);
	return true;
}

bool CNumber::SetTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	if (mOwner->GetViewport())
	{
		if (!mOwner->GetViewport()->GetScene()->GetResource()->LoadTexture(name, vecFileName, pathName))
		{
			assert(false);
			return false;
		}
		mInfo.Texture = mOwner->GetViewport()->GetScene()->GetResource()->FindTexture(name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName);
		mInfo.Texture = CResourceManager::GetInst()->FindTexture(name);
	}
	SetUseTexture(true);
}

bool CNumber::SetTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
	if (mOwner->GetViewport())
	{
		if (!mOwner->GetViewport()->GetScene()->GetResource()->LoadTexture(name, vecFullPath))
		{
			assert(false);
			return false;
		}
		mInfo.Texture = mOwner->GetViewport()->GetScene()->GetResource()->FindTexture(name);
	}
	else
	{
		CResourceManager::GetInst()->LoadTexture(name, vecFullPath);
		mInfo.Texture = CResourceManager::GetInst()->FindTexture(name);
	}
	SetUseTexture(true);
	return true;
}

void CNumber::SetTextureTint(const Vector4& tint)
{
	mInfo.Tint = tint;
}

void CNumber::SetTextureTint(const float r, const float g, const float b, const float a)
{
	mInfo.Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CNumber::AddFrameData(const Vector2& start, const Vector2& size)
{
	AnimationFrameData data;
	data.Start = start;
	data.Size = size;
	mInfo.vecFrameData.push_back(data);
}

void CNumber::AddFrameDataFrameTexture(const int count)
{
	for (int i = 0; i < count; ++i)
	{
		AnimationFrameData data;
		data.Start = Vector2(0.f, 0.f);
		data.Size = Vector2((float)mInfo.Texture->GetWidth(i), (float)mInfo.Texture->GetHeight(i));
		mInfo.vecFrameData.push_back(data);
	}
}
