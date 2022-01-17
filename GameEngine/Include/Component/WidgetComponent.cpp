#include "WidgetComponent.h"
#include "CameraComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "../Device.h"
#include "../Engine.h"

CWidgetComponent::CWidgetComponent()
{
	SetTypeID<CWidgetComponent>();

	meSpace = eWidgetComponentSpace::Screen;

	SetBoolInheritRotX(false);
	SetBoolInheritRotY(false);
	SetBoolInheritRotZ(false);
	
	mbIsRender = true;
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& com)
{
	meSpace = com.meSpace;

	if (eWidgetComponentSpace::Screen == meSpace)
	{
		SetBoolInheritRotX(false);
		SetBoolInheritRotY(false);
		SetBoolInheritRotZ(false);
	}
	else
	{
		SetBoolInheritRotX(true);
		SetBoolInheritRotY(true);
		SetBoolInheritRotZ(true);
	}
	mWidgetWindow = com.mWidgetWindow->Clone();
	mWidgetWindow->mOwnerComponent = this;
}

CWidgetComponent::~CWidgetComponent()
{
}

bool CWidgetComponent::Init()
{
	SetLayerName("ScreenWidgetComponent");
	return true;
}

void CWidgetComponent::Start()
{
	CSceneComponent::Start();
}

void CWidgetComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	if (mWidgetWindow)
	{
		mWidgetWindow->Update(deltaTime);
	}
}

void CWidgetComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);

	if (mWidgetWindow)
	{
		// 월드공간에서의 위치 정보로 WidgetComponent의 위치 정보가 갱신되어 있을 것이다.
		// 이를 화면공간에서의 위치로 변환해 주어야 한다.
		// 월드 -> 뷰 -> 투영 변환 후에 화면공간으로 변환이 된다.
		// 화면공간으로 변환이 될때에는 투영으로 변환된 x, y, z, w 가 있을 경우 모든 요소들을
		// w로 나누어서 화면공간으로 변환을 시도한다.
		CCameraComponent* camera = mScene->GetCameraManager()->GetCurrentCamera();

		Matrix matView = camera->GetViewMatrix();
		Matrix matProj = camera->GetProjMatrix();
		Matrix matVP = matView * matProj;

		Vector3 worldPos = GetWorldPos();
		Vector3 viewPos = worldPos.TransformCoord(matView);
		Vector3 projPos = viewPos.TransformCoord(matProj); // z로 나누기 전 좌표

		if (eWidgetComponentSpace::World == meSpace)
		{
			// 2D에서는 항상 1 -> 투영행렬의 3행4열의 1값과 2D좌표의 z 값과 곱하기 때문
			float w = viewPos.x * matProj._14 + viewPos.y * matProj._24 + viewPos.z * matProj._34 + matProj._44;

			// z로 나눠서, 투영공간 좌표로 변환
			projPos.x /= w;
			projPos.y /= w;
			projPos.z /= w;
		}
		
		Resolution RS = CDevice::GetInst()->GetResolution();

		// 2D
		Vector2 screenPos;
		screenPos.x = projPos.x * (RS.Width / 2.f) + (RS.Width / 2.f);
		screenPos.y = projPos.y * (RS.Height / 2.f) + (RS.Height / 2.f);

		mWidgetWindow->SetPos(screenPos);

		mWidgetWindow->PostUpdate(deltaTime);
	}
}

void CWidgetComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CWidgetComponent::Render()
{
	CSceneComponent::Render();
	
	mWidgetWindow->Render();
}

void CWidgetComponent::PostRender()
{
	CSceneComponent::PostRender();
}

void CWidgetComponent::Save(FILE* fp)
{
	CSceneComponent::Save(fp);
}

void CWidgetComponent::Load(FILE* fp)
{
	CSceneComponent::Load(fp);
}

CWidgetComponent* CWidgetComponent::Clone()
{
	return new CWidgetComponent(*this);
}
