#include "Widget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
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
	mbMouseHovered(false)
{
}

CWidget::CWidget(const CWidget& widget)
{
	mbMouseHovered = false;
}

CWidget::~CWidget()
{
	SAFE_DELETE(mCBuffer);
}

bool CWidget::Init()
{
	mShader = mOwner->GetViewport()->GetScene()->GetResource()->FindShader("WidgetShader");
	mMesh = mOwner->GetViewport()->GetScene()->GetResource()->FindMesh("WidgetMesh");

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
	matTrans.Translation(mPos.x, mPos.y, 0.f);

	CCameraComponent* cam = mOwner->GetViewport()->GetScene()->GetCameraManager()->GetUICamera();

	// �׻� ���� ��ġ�� ����ϱ� ������, ����� ��ȯ �ʿ� ����
	Matrix matWP = matScale * matRot * matTrans * cam->GetProjMatrix();
	matWP.Transpose();

	// Render
	mCBuffer->SetWP(matWP);
	mCBuffer->SetTint(mTint);
	mCBuffer->UpdateCBuffer();
	mShader->SetShader();
	mMesh->Render();
}

bool CWidget::DoCollideMouse(const Vector2& mousePos)
{
	if (mousePos.x < mPos.x)
	{
		return false;
	}
	else if (mousePos.x > mPos.x + mSize.x)
	{
		return false;
	}
	else if (mousePos.y < mPos.y)
	{
		return false;
	}
	else if (mousePos.y > mPos.y + mSize.y)
	{
		return false;
	}
	return true;
}

void CWidget::SetShader(const std::string& name)
{
	mShader = mOwner->GetViewport()->GetScene()->GetResource()->FindShader(name);
}

void CWidget::SetUseTexture(bool bUse)
{
	mCBuffer->SetUseTextre(bUse);
}
