#include "ColliderBox2D.h"
#include "../Collision/Collision.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "CameraComponent.h"
#include "ColliderCircle.h"
#include "ColliderPixel.h"
#include "../Engine.h"

CColliderBox2D::CColliderBox2D()
{
	SetTypeID<CColliderBox2D>();
	meComponentType = eComponentType::SCENE_COMP;
	mbIsRender = true;
	meColliderType = eColliderType::Box2D;
}

CColliderBox2D::CColliderBox2D(const CColliderBox2D& com)	:
	CColliderComponent(com)
{
	mInfo = com.mInfo;
}

CColliderBox2D::~CColliderBox2D()
{
}

bool CColliderBox2D::Init()
{
	if (!CColliderComponent::Init())
	{
		assert(false);
		return false;
	}
	
	// default
	mInfo.Axis[0] = Vector2(1.f, 0.f);
	mInfo.Axis[1] = Vector2(0.f, 1.f);

	mInfo.Length = Vector2(50.f, 50.f);

	// �θ��� ȸ���� ������ ����
	SetBoolInheritRotZ(true);

	// Length�� ��ü �������� ��
	SetWorldScale(mInfo.Length.x * 2.f, mInfo.Length.y * 2.f, 1.f);

	// ����� ���� Mesh
	mMesh = mScene->GetResource()->FindMesh("Box2DMesh");

	return true;
}

void CColliderBox2D::Start()
{
	CColliderComponent::Start();

	SetBoolInheritRotZ(true);
	SetWorldScale(mInfo.Length.x * 2.f, mInfo.Length.y * 2.f, 1.f);

	if (!mMesh)
	{
		mMesh = mScene->GetResource()->FindMesh("Box2DMesh");
	}
}

void CColliderBox2D::Update(float deltaTime)
{
	CColliderComponent::Update(deltaTime);
}

void CColliderBox2D::PostUpdate(float deltaTime)
{
	CColliderComponent::PostUpdate(deltaTime);

	// �������� ���� �� �浹ü�� �� ��ǥ�� �����Ѵ�.
	mInfo.Center.x = GetWorldPos().x + mOffset.x;
	mInfo.Center.y = GetWorldPos().y + mOffset.y;

	mInfo.Axis[0].x = GetWorldAxis(AXIS_X).x;
	mInfo.Axis[0].y = GetWorldAxis(AXIS_X).y;

	mInfo.Axis[1].x = GetWorldAxis(AXIS_Y).x;
	mInfo.Axis[1].y = GetWorldAxis(AXIS_Y).y;

	// �� ������ ��ǥ�� ���Ѵ�.
	Vector2 pos[4] = {};
	pos[0] = mInfo.Center - mInfo.Axis[0] * mInfo.Length.x + mInfo.Axis[1] * mInfo.Length.y; // �»�
	pos[1] = mInfo.Center + mInfo.Axis[0] * mInfo.Length.x + mInfo.Axis[1] * mInfo.Length.y; // ���
	pos[2] = mInfo.Center - mInfo.Axis[0] * mInfo.Length.x - mInfo.Axis[1] * mInfo.Length.y; // ����
	pos[3] = mInfo.Center + mInfo.Axis[0] * mInfo.Length.x - mInfo.Axis[1] * mInfo.Length.y; // ����

	mMinPos.x = pos[0].x;
	mMinPos.y = pos[0].y;
	mMaxPos.x = pos[0].x;
	mMaxPos.y = pos[0].y;

	// �ִ�, �ּҰ� �Ǻ�
	for (int i = 1; i < 4; ++i)
	{
		if (mMinPos.x > pos[i].x)
		{
			mMinPos.x = pos[i].x;
		}

		if (mMinPos.y > pos[i].y)
		{
			mMinPos.y = pos[i].y;
		}

		if (mMaxPos.x < pos[i].x)
		{
			mMaxPos.x = pos[i].x;
		}

		if (mMaxPos.y < pos[i].y)
		{
			mMaxPos.y = pos[i].y;
		}
	}

	// ���� �ּ�, �ִ� ��ǥ �Ǻ�
	mInfo.Min.x = mMinPos.x;
	mInfo.Min.y = mMinPos.y;
	mInfo.Max.x = mMaxPos.x;
	mInfo.Max.y = mMaxPos.y;
}

void CColliderBox2D::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderBox2D::Render()
{
	CColliderComponent::Render();

	if (CEngine::GetInst()->IsDebugMode())
	{
		CCameraComponent* cam = mScene->GetCameraManager()->GetCurrentCamera();

		Matrix matWorld, matView, matProj, matWVP;
		matView = cam->GetViewMatrix();
		matProj = cam->GetProjMatrix();

		Matrix matScale, matRot, matTrans;
		matScale.Scaling(mInfo.Length.x * 2.f, mInfo.Length.y * 2.f, 1.f);
		matRot.Rotation(GetWorldRot());
		matTrans.Translation(mInfo.Center);
		
		matWorld = matScale * matRot * matTrans;

		matWVP = matWorld * matView * matProj;

		matWVP.Transpose();

		mCBuffer->SetColliderWVP(matWVP);

		if (mPrevCollisionList.empty())
		{
			mCBuffer->SetColliderColor(Vector4(0.f, 1.f, 0.f, 1.f));
		}
		else
		{
			mCBuffer->SetColliderColor(Vector4(1.f, 0.f, 0.f, 1.f));
		}

		if (mbMouseCollision)
		{
			mCBuffer->SetColliderColor(Vector4(1.f, 0.f, 0.f, 1.f));
		}

		mCBuffer->UpdateCBuffer();
		mShader->SetShader();
		mMesh->Render();
	}
}

void CColliderBox2D::PostRender()
{
	CColliderComponent::PostRender();
}

CColliderBox2D* CColliderBox2D::Clone()
{
	return new CColliderBox2D(*this);
}

void CColliderBox2D::Save(FILE* fp)
{
	CColliderComponent::Save(fp);
	fwrite(&mInfo, sizeof(Box2DInfo), 1, fp);
}

void CColliderBox2D::Load(FILE* fp)
{
	CColliderComponent::Load(fp);
	fread(&mInfo, sizeof(Box2DInfo), 1, fp);
}

bool CColliderBox2D::DoCollide(CColliderComponent* dest)
{
	switch (dest->GetColliderType())
	{
	case eColliderType::Box2D:
		return CCollision::CollisionBox2DToBox2D(this, (CColliderBox2D*)dest);
	case eColliderType::Circle2D:
		return CCollision::CollisionBox2DToCircle(this, (CColliderCircle*)dest);
	case eColliderType::Pixel:
		return CCollision::CollisionBox2DToPixel(this, (CColliderPixel*)dest);
	default:
		assert(false);
		return false;
	}
}

bool CColliderBox2D::DoCollideMouse(const Vector2& mousePos)
{
	CollisionResult result;
	mbMouseCollision = CCollision::CollisionBox2DToPoint(mMouseResult, result, mInfo, mousePos);
	return mbMouseCollision;
}
