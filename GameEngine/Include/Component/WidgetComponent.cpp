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
		// ������������� ��ġ ������ WidgetComponent�� ��ġ ������ ���ŵǾ� ���� ���̴�.
		// �̸� ȭ����������� ��ġ�� ��ȯ�� �־�� �Ѵ�.
		// ���� -> �� -> ���� ��ȯ �Ŀ� ȭ��������� ��ȯ�� �ȴ�.
		// ȭ��������� ��ȯ�� �ɶ����� �������� ��ȯ�� x, y, z, w �� ���� ��� ��� ��ҵ���
		// w�� ����� ȭ��������� ��ȯ�� �õ��Ѵ�.
		CCameraComponent* camera = mScene->GetCameraManager()->GetCurrentCamera();

		Matrix matView = camera->GetViewMatrix();
		Matrix matProj = camera->GetProjMatrix();
		Matrix matVP = matView * matProj;

		Vector3 worldPos = GetWorldPos();
		Vector3 viewPos = worldPos.TransformCoord(matView);
		Vector3 projPos = viewPos.TransformCoord(matProj); // z�� ������ �� ��ǥ

		if (eWidgetComponentSpace::World == meSpace)
		{
			// 2D������ �׻� 1 -> ��������� 3��4���� 1���� 2D��ǥ�� z ���� ���ϱ� ����
			float w = viewPos.x * matProj._14 + viewPos.y * matProj._24 + viewPos.z * matProj._34 + matProj._44;

			// z�� ������, �������� ��ǥ�� ��ȯ
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