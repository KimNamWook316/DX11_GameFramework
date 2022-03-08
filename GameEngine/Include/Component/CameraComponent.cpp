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
		�� ���������� ��ȯ��, ī�޶� ��ǥ�� �������� ��ü�� �����ϴ� ��
		����, ī�޶��� x, y, z���� ������ ��� ��ġ��Ų��.
		�׷���, ���� ���� ������ �����Ǿ� �ִ� ��ü�鵵, ���� �������־�� �Ѵ�.
		ī�޶��� ���� x, y, z �̵� ����� �ݴ�� ���� ���� �� �̵����
		�׷��� ī�޶��� x, y,z���� ���� ��� ��ġ�Ѵ�.
		�� ��, ī�޶��� ȸ���� ���忡 ���߰� �Ǹ鼭, ��ü���� ī�޶� �߽����� �׸�ŭ �����Ѵ�.

		ī�޶� ��������� ��ȯ�� �Ǹ� ī�޶� �����ϴ� x, y, z ���� ������ ��� ��ġ�ϰ�
		�ȴ�. ��, ���� x : 1, 0, 0  y : 0, 1, 0  z : 0, 0, 1 �� ������ �ȴٴ� ���̴�.
		������� : ����� �����ϴ� 3���� ���� ��� ���ο� ���� ������ �ϴ� ����� ���Ѵ�.
		1 0 0
		0 1 0
		0 0 1

		��������� ��ġ ��İ� ������� ����.

		ī�޶� X�� * ����� = 1, 0, 0
		ī�޶� y�� * ����� = 0, 1, 0
		ī�޶� z�� * ����� = 0, 0, 1
		x, y, z   0 0 0   1 0 0
		x, y, z * 0 0 0 = 0 1 0
		x, y, z	  0 0 0   0 0 1

		���� ȸ�� ���
		Xx Yx Zx 0
		Xy Yy Zy 0
		Xz Yz Zz 0
		0  0  0  1

		���� �̵� ���
		1   0  0 0
		0   1  0 0
		0   0  1 0
		-x -y -z 1

		1   0  0 0   Xx Yx Zx 0
		0   1  0 0 * Xy Yy Zy 0
		0   0  1 0   Xz Yz Zz 0
		-x -y -z 1   0  0  0  1

		���� �� ���
		Xx    Yx   Zx  0
		Xy    Yy   Zy  0
		Xz    Yz   Zz  0
		-X.P -Y.P -Z.P 1
		*/
}

void CCameraComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
	
	// Update���� Camera�� �پ� �̵��ϴ� ������Ʈ�� Transform�� ��ȭ�� ���� �� �����Ƿ�
	// ��� ������Ʈ�� Transform�� ������ ���Ŀ� ī�޶� ó���Ѵ�.
	mMatView.Identity();

	// ī�޶� ȸ����
	for (int i = 0; i < eAXIS::AXIS_MAX; ++i)
	{
		Vector3 axis = GetWorldAxis((eAXIS)i);
		memcpy(&mMatView[i][0], &axis, sizeof(Vector3));
	}
	// ���� ��� �����ϹǷ�, ��ġ�ؼ� ����� �����.
	mMatView.Transpose();

	// ���̵�
	Vector3 pos = GetWorldPos() * -1.f;
	
	for (int i = 0; i < eAXIS::AXIS_MAX; ++i)
	{
		Vector3 axis = GetWorldAxis((eAXIS)i);
		// Update() �ּ� ���� 
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
