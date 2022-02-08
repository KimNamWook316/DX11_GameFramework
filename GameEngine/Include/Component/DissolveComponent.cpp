#include "DissolveComponent.h"
#include "../Resource/Material/Material.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"

CDissolveComponent::CDissolveComponent()
{
	mCBuffer = new CDissolveConstantBuffer;
	mCBuffer->Init();

	mFinishTime = 5.f;
	mbStart = false;
	mFilter = 0.f;
}

CDissolveComponent::CDissolveComponent(CDissolveComponent& com)	:
	CObjectComponent(com)
{
	mCBuffer = com.mCBuffer->Clone();

	mFinishTime = com.mFinishTime;
	mMaterial = com.mMaterial;
	mbStart = false;
	mFilter = 0.f;
}

CDissolveComponent::~CDissolveComponent()
{
	SAFE_DELETE(mCBuffer);

	if (mMaterial)
	{
		mMaterial->DeleteRenderCallBack(this);
	}
}

bool CDissolveComponent::Init()
{
	SetDissolveTexture("DefaultDissolveTexture", TEXT("DefaultPaperBurn.png"));
	return true;
}

void CDissolveComponent::Start()
{
	CObjectComponent::Start();
}

void CDissolveComponent::Update(float deltaTime)
{
	if (mbStart)
	{
		// 0 ~ 1
		mFilter += deltaTime / mFinishTime;

		if (mFilter >= 1.f)
		{
			mFilter = 1.f;

			if (mFinishCallBack)
			{
				mFinishCallBack();
			}
		}
		
		mCBuffer->SetFilter(mFilter);
	}
}

void CDissolveComponent::PostUpdate(float deltaTime)
{
}

void CDissolveComponent::PrevRender()
{
}

void CDissolveComponent::Render()
{
}

void CDissolveComponent::PostRender()
{
}

CDissolveComponent* CDissolveComponent::Clone()
{
	return new CDissolveComponent(*this);
}

void CDissolveComponent::StartDissolve()
{
	if (!mMaterial || mbStart)
	{
		return;
	}

	mMaterial->SetDissolve(true);
	mbStart = true;
}

void CDissolveComponent::SetShader()
{
	mCBuffer->UpdateCBuffer();
	mDissolveTexture->SetShader(101, (int)eBufferShaderTypeFlags::Pixel, 0);
}

void CDissolveComponent::SetMaterial(CMaterial* material)
{
	mMaterial = material;
	mMaterial->AddRenderCallBack(this, &CDissolveComponent::SetShader);
}

void CDissolveComponent::SetInverse(bool bEnable)
{
	mCBuffer->SetInverse(bEnable);
}

void CDissolveComponent::SetDissolveTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	if (!mScene->GetResource()->LoadTexture(name, fileName, pathName))
	{
		assert(false);
		return;
	}

	mDissolveTexture = mScene->GetResource()->FindTexture(name);
}

void CDissolveComponent::SetOutLineColor(const Vector4& color)
{
	mCBuffer->SetOutColor(color);
}

void CDissolveComponent::SetOutLineColor(const float r, const float g, const float b, const float a)
{
	mCBuffer->SetOutColor(r,g,b,a);
}

void CDissolveComponent::SetInLineColor(const Vector4& color)
{
	mCBuffer->SetInColor(color);
}

void CDissolveComponent::SetInLineColor(const float r, const float g, const float b, const float a)
{
	mCBuffer->SetInColor(r,g,b,a);
}

void CDissolveComponent::SetCenterLineColor(const Vector4& color)
{
	mCBuffer->SetCenterColor(color);
}

void CDissolveComponent::SetCenterLineColor(const float r, const float g, const float b, const float a)
{
	mCBuffer->SetCenterColor(r,g,b,a);
}

void CDissolveComponent::SetInFilter(const float filter)
{
	mCBuffer->SetInFilter(filter);
}

void CDissolveComponent::SetOutFilter(const float filter)
{
	mCBuffer->SetOutFilter(filter);
}

void CDissolveComponent::SetCenterFilter(const float filter)
{
	mCBuffer->SetCenterFilter(filter);
}
