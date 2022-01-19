#include "Widget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/CameraManager.h"
#include "../Component/CameraComponent.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"

CWidget::CWidget() :
	mOwner(nullptr),
	mbStart(false),
	mCBuffer(nullptr),
	mZOrder(0),
	mTint(255.f, 255.f, 255.f, 255.f),
	mAngle(0.f),
	mSize(50.f, 50.f),
	mbMouseHovered(false),
	mbCollisionMouseEnable(true)
{
}

CWidget::CWidget(const CWidget& widget)
{
	*this = widget;

	mRefCount = 0;
	mOwner = nullptr;
	mbMouseHovered = false;
	mCBuffer = new CWidgetConstantBuffer;
	mCBuffer->Init();
}

CWidget::~CWidget()
{
	SAFE_DELETE(mCBuffer);
}

bool CWidget::Init()
{
	if (mOwner->GetViewport())
	{
		mShader = mOwner->GetViewport()->GetScene()->GetResource()->FindShader("WidgetShader");
		mMesh = mOwner->GetViewport()->GetScene()->GetResource()->FindMesh("WidgetMesh");
	}
	else
	{
		mShader = CResourceManager::GetInst()->FindShader("WidgetShader");
		mMesh = CResourceManager::GetInst()->FindMesh("WidgetMesh");
	}

	mCBuffer = new CWidgetConstantBuffer;
	mCBuffer->Init();

	return true;
}

void CWidget::Start()
{
	mbStart = true;
}

void CWidget::Update(float deltaTime)
{
	if (!mbStart)
	{
		Start();
	}
}

void CWidget::PostUpdate(float deltaTime)
{
	if (!mbStart)
	{
		Start();
	}

	mRenderPos = mPos;

	if (mOwner)
	{
		mRenderPos += mOwner->GetWindowPos();
	}
}

void CWidget::Render()
{
	if (!mbStart)
	{
		Start();
	}

	Matrix matScale, matRot, matTrans;
	matScale.Scaling(mSize.x, mSize.y, 1.f);
	matRot.Rotation(0.f, 0.f, mAngle);
	matTrans.Translation(mRenderPos.x, mRenderPos.y, 0.f);

	CCameraComponent* cam = nullptr;

	if (mOwner->GetViewport())
	{
		cam = mOwner->GetViewport()->GetScene()->GetCameraManager()->GetUICamera();
	}
	else
	{
		cam = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetUICamera();
	}

	if (!cam)
	{
		assert(false);
	}

	// 항상 고정 위치에 출력하기 때문에, 뷰공간 변환 필요 없음
	Matrix matWP = matScale * matRot * matTrans * cam->GetProjMatrix();
	matWP.Transpose();

	// Render
	mCBuffer->SetWP(matWP);
	mCBuffer->SetTint(mTint);
	mCBuffer->UpdateCBuffer();
	mShader->SetShader();
	mMesh->Render();
}

CWidget* CWidget::Clone()
{
	return new CWidget(*this);
}

bool CWidget::DoCollideMouse(const Vector2& mousePos)
{
	if (!mbCollisionMouseEnable)
	{
		return false;
	}

	if (mousePos.x < mRenderPos.x)
	{
		return false;
	}
	else if (mousePos.x > mRenderPos.x + mSize.x)
	{
		return false;
	}
	else if (mousePos.y < mRenderPos.y)
	{
		return false;
	}
	else if (mousePos.y > mRenderPos.y + mSize.y)
	{
		return false;
	}
	return true;
}

void CWidget::SetShader(const std::string& name)
{
	if (mOwner->GetViewport())
	{
		mShader = mOwner->GetViewport()->GetScene()->GetResource()->FindShader(name);
	}
	else
	{
		mShader = CResourceManager::GetInst()->FindShader(name);
	}
}

void CWidget::SetUseTexture(bool bUse)
{
	mCBuffer->SetUseTextre(bUse);
}
