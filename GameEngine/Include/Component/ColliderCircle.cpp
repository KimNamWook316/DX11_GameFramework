#include "ColliderCircle.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "CameraComponent.h"
#include "ColliderBox2D.h"
#include "../Collision/Collision.h"
#include "ColliderPixel.h"
#include "../Engine.h"

bool CColliderCircle::DoCollide(CColliderComponent* dest)
{
	switch (dest->GetColliderType())
	{
	case eColliderType::Box2D:
		return CCollision::CollisionBox2DToCircle((CColliderBox2D*)dest, this);
	case eColliderType::Circle2D:
		return CCollision::CollisionCircleToCircle(this, (CColliderCircle*)dest);
	case eColliderType::Pixel:
		return CCollision::CollisionCircleToPixel(this, (CColliderPixel*)dest);
	default:
		assert(false);
		return false;
	}
	return false;
}

bool CColliderCircle::DoCollideMouse(const Vector2& mousePos)
{
	CollisionResult result;
	mbMouseCollision = CCollision::CollisionCircleToPoint(mMouseResult, result, mInfo, mousePos);
	return mbMouseCollision;
}

CColliderCircle::CColliderCircle()
{
	SetTypeID<CColliderCircle>();
	meColliderType = eColliderType::Circle2D;
	meComponentType = eComponentType::SCENE_COMP;
	mbIsRender = true;
}

CColliderCircle::CColliderCircle(const CColliderCircle& com)	:
	CColliderComponent(com)
{
	mInfo = com.mInfo;
	SetTypeID<CColliderCircle>();
	meColliderType = eColliderType::Circle2D;
	meComponentType = eComponentType::SCENE_COMP;
}

CColliderCircle::~CColliderCircle()
{
}

bool CColliderCircle::Init()
{
	if (!CColliderComponent::Init())
	{
		assert(false);
		return false;
	}

	mInfo.Radius = 50.f;
	SetBoolInheritRotZ(true);
	SetWorldScale(mInfo.Radius * 2.f, mInfo.Radius * 2.f, 1.f);
	mMesh = mScene->GetResource()->FindMesh("CircleMesh");

	return true;
}

void CColliderCircle::Start()
{
	CColliderComponent::Start();

	SetBoolInheritRotZ(true);
	SetWorldScale(mInfo.Radius * 2.f, mInfo.Radius * 2.f, 1.f);

	if (!mMesh)
	{
		mMesh = mScene->GetResource()->FindMesh("CircleMesh");
	}
}

void CColliderCircle::Update(float deltaTime)
{
	CColliderComponent::Update(deltaTime);
}

void CColliderCircle::PostUpdate(float deltaTime)
{
	CColliderComponent::PostUpdate(deltaTime);

	mInfo.Center.x = GetWorldPos().x + mOffset.x;
	mInfo.Center.y = GetWorldPos().y + mOffset.y;

	mMaxPos.x = mInfo.Center.x + mInfo.Radius;
	mMaxPos.y = mInfo.Center.y + mInfo.Radius;

	mMinPos.x = mInfo.Center.x - mInfo.Radius;
	mMinPos.y = mInfo.Center.y - mInfo.Radius;

	mInfo.Max.x = mMaxPos.x;
	mInfo.Max.y = mMaxPos.y;

	mInfo.Min.x = mMinPos.x;
	mInfo.Min.y = mMinPos.y;
}

void CColliderCircle::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderCircle::Render()
{
	CColliderComponent::Render();

	if (CEngine::GetInst()->IsDebugMode())
	{
		CCameraComponent* cam = mScene->GetCameraManager()->GetCurrentCamera();

		Matrix matView, matProj;
		matView = cam->GetViewMatrix();
		matProj = cam->GetProjMatrix();

		Matrix matScale, matTrans, matWorld, matWVP;
		matScale.Scaling(mInfo.Radius * 2.f, mInfo.Radius * 2.f, 1.f);
		matTrans.Translation(mInfo.Center);

		matWorld = matScale * matTrans;
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

void CColliderCircle::PostRender()
{
	CColliderComponent::PostRender();
}

CColliderCircle* CColliderCircle::Clone()
{
	return new CColliderCircle(*this);
}

void CColliderCircle::Save(FILE* fp)
{
	CColliderComponent::Save(fp);
	fwrite(&mInfo, sizeof(CircleInfo), 1, fp);
}

void CColliderCircle::Load(FILE* fp)
{
	CColliderComponent::Load(fp);
	fread(&mInfo, sizeof(CircleInfo), 1, fp);
}
