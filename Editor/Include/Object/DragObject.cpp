#include "DragObject.h"
#include "Component/StaticMeshComponent.h"
#include "Animation/AnimationSequence2DInstance.h"

CDragObject::CDragObject() : 
	mMeshComponent(nullptr)
{
}

CDragObject::CDragObject(const CDragObject& obj)	:
	CGameObject(obj)
{
	mMeshComponent = (CStaticMeshComponent*)FindComponent("Mesh");
}

CDragObject::~CDragObject()
{
}

bool CDragObject::Init()
{
	mMeshComponent = CreateComponent<CStaticMeshComponent>("Mesh");

	SetRootSceneComponent(mMeshComponent);

	mMeshComponent->SetMesh("FrameRectMesh");
	mMeshComponent->GetMaterial()->SetShader("PosMeshShader");
	mMeshComponent->SetPivot(0.f, 1.f, 0.f);
	mMeshComponent->SetBaseColor(0.f, 1.f, 0.f, 1.f);

	mRootSceneComponent->SetLayerName("DragLayer");

	return true;
}

void CDragObject::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CDragObject::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CDragObject* CDragObject::Clone()
{
	return new CDragObject(*this);
}

void CDragObject::SetStartPos(const Vector2& pos)
{
	mStartPos = pos;
	SetWorldPos(pos.x, pos.y, 0.f);
}

void CDragObject::SetEndPos(const Vector2& pos)
{
	mEndPos = pos;
	SetWorldScale(mEndPos.x - mStartPos.x, mEndPos.y - mStartPos.y, 1.f);
}
