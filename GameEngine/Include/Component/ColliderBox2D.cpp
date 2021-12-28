#include "ColliderBox2D.h"
#include "../Collision/Collision.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

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
}

void CColliderBox2D::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderBox2D::Render()
{
	CColliderComponent::Render();
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

bool CColliderBox2D::DoCollide(CColliderComponent* Dest)
{
	return false;
}

bool CColliderBox2D::DoCollideMouse(const Vector2& mousePos)
{
	CollisionResult result;
	mbMouseCollision = CCollision::CollisionBox2DToPoint(mMouseResult, result, mInfo, mousePos);
	return mbMouseCollision;
}
