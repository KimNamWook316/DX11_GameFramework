#include "CameraComponent.h"
#include "../Device.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"

CCameraComponent::CCameraComponent()
{
	SetTypeID<CCameraComponent>();
	mbIsRender = false;
	meCameraType = eCameraType::Camera2D;
	mViewAngle = 90.f;
	mDistance = 1000.f;
	mScale = Vector3(1.f, 1.f, 1.f);
}

CCameraComponent::CCameraComponent(const CCameraComponent& com)
{
	SetTypeID<CCameraComponent>();
	mbIsRender = false;
	meCameraType = com.meCameraType;
	mViewAngle = com.mViewAngle;
	mDistance = com.mDistance;
	mMatView = com.mMatView;
	mMatProj = com.mMatProj;
	mRS = com.mRS;
}

CCameraComponent::~CCameraComponent()
{
}

bool CCameraComponent::Init()
{
	CSceneComponent::Init();
	mRS = CDevice::GetInst()->GetResolution();
	return true;
}

void CCameraComponent::Start()
{
	CSceneComponent::Start();
	mRS = CDevice::GetInst()->GetResolution();
	CreateProjectionMatrix();
}

void CCameraComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	/*
		NOTE
		뷰 공간으로의 변환은, 카메라 좌표계 기준으로 물체를 구성하는 것
		먼저, 카메라의 x, y, z축을 월드의 축과 일치시킨다.
		그러면, 현재 월드 공간에 구성되어 있는 물체들도, 따라서 움직여주어야 한다.
		카메라의 현재 x, y, z 이동 행렬을 반대로 해준 것이 뷰 이동행렬
		그러면 카메라의 x, y,z축은 월드 축과 일치한다.
		그 후, 카메라의 회전을 월드에 맞추게 되면서, 물체들은 카메라를 중심으로 그만큼 공전한다.

		카메라가 뷰공간으로 변환이 되면 카메라를 구성하는 x, y, z 축은 월드의 축과 일치하게
		된다. 즉, 축은 x : 1, 0, 0  y : 0, 1, 0  z : 0, 0, 1 로 구성이 된다는 것이다.
		직교행렬 : 행렬을 구성하는 3개의 축이 모두 서로에 대해 정직교 하는 행렬을 말한다.
		1 0 0
		0 1 0
		0 0 1

		직교행렬은 전치 행렬과 역행렬이 같다.

		카메라 X축 * 뷰행렬 = 1, 0, 0
		카메라 y축 * 뷰행렬 = 0, 1, 0
		카메라 z축 * 뷰행렬 = 0, 0, 1
		x, y, z   0 0 0   1 0 0
		x, y, z * 0 0 0 = 0 1 0
		x, y, z	  0 0 0   0 0 1

		뷰의 회전 행렬
		Xx Yx Zx 0
		Xy Yy Zy 0
		Xz Yz Zz 0
		0  0  0  1

		뷰의 이동 행렬
		1   0  0 0
		0   1  0 0
		0   0  1 0
		-x -y -z 1

		1   0  0 0   Xx Yx Zx 0
		0   1  0 0 * Xy Yy Zy 0
		0   0  1 0   Xz Yz Zz 0
		-x -y -z 1   0  0  0  1

		최종 뷰 행렬
		Xx    Yx   Zx  0
		Xy    Yy   Zy  0
		Xz    Yz   Zz  0
		-X.P -Y.P -Z.P 1
		*/
}

void CCameraComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
	
	// Update에서 Camera가 붙어 이동하는 오브젝트의 Transform에 변화가 있을 수 있으므로
	// 모든 오브젝트의 Transform이 결정된 이후에 카메라를 처리한다.
	mMatView.Identity();

	// 카메라 회전값
	for (int i = 0; i < eAXIS::AXIS_MAX; ++i)
	{
		Vector3 axis = GetWorldAxis((eAXIS)i);
		memcpy(&mMatView[i][0], &axis, sizeof(Vector3));
	}
	// 축은 모두 직교하므로, 전치해서 역행렬 만든다.
	mMatView.Transpose();

	// 역이동
	Vector3 pos = GetWorldPos() * -1.f;
	
	for (int i = 0; i < eAXIS::AXIS_MAX; ++i)
	{
		Vector3 axis = GetWorldAxis((eAXIS)i);
		// Update() 주석 참조 
		mMatView[3][i] = pos.Dot(axis);
	}

	mMatView[1][1] *= mScale.x;
	mMatView[2][2] *= mScale.y;
	mMatView[3][3] *= mScale.z;
}

void CCameraComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CCameraComponent::Render()
{
	CSceneComponent::Render();
}

void CCameraComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CCameraComponent* CCameraComponent::Clone()
{
	return new CCameraComponent(*this);
}

void CCameraComponent::Save(FILE* fp)
{
	CSceneComponent::Save(fp);

	fwrite(&meCameraType, sizeof(meCameraType), 1, fp);
	fwrite(&mMatProj, sizeof(Matrix), 1, fp);
	fwrite(&mMatView, sizeof(Matrix), 1, fp);
	fwrite(&mViewAngle, sizeof(float), 1, fp);
	fwrite(&mDistance, sizeof(float), 1, fp);
	fwrite(&mRS, sizeof(Resolution), 1, fp);

	bool bIsPlayerCamera = false;
	if (mObject == mObject->GetScene()->GetPlayerObj())
	{
		bIsPlayerCamera = true;
	}

	fwrite(&bIsPlayerCamera, sizeof(bool), 1, fp);
}

void CCameraComponent::Load(FILE* fp)
{
	CSceneComponent::Load(fp);

	fread(&meCameraType, sizeof(meCameraType), 1, fp);
	fread(&mMatProj, sizeof(Matrix), 1, fp);
	fread(&mMatView, sizeof(Matrix), 1, fp);
	fread(&mViewAngle, sizeof(float), 1, fp);
	fread(&mDistance, sizeof(float), 1, fp);
	fread(&mRS, sizeof(Resolution), 1, fp);

	bool bIsPlayerCamera = false;
	fread(&bIsPlayerCamera, sizeof(bool), 1, fp);

	if (bIsPlayerCamera)
	{
		mObject->GetScene()->GetCameraManager()->KeepCamera();
		mObject->GetScene()->GetCameraManager()->SetCurrentCamera(this);
	}
}

void CCameraComponent::CreateProjectionMatrix()
{
	switch (meCameraType)
	{
	case eCameraType::Camera2D:
		mMatProj = XMMatrixOrthographicOffCenterLH(0.f, (float)mRS.Width, 0.f, (float)mRS.Height, 0.f, 1000.f);
		break;
	case eCameraType::Camera3D:
		mMatProj = XMMatrixPerspectiveFovLH(DegToRad(mViewAngle), (float)(mRS.Width / mRS.Height), 0.1f, mDistance);
		break;
	case eCameraType::UI:
		mMatProj = XMMatrixOrthographicOffCenterLH(0.f, (float)mRS.Width, 0.f, (float)mRS.Height, 0.f, 1000.f);
		break;
	default:
		assert(false);
		break;
	}
}
