#include "D2ProjectTile.h"
#include "GameObject/GameObject.h"
#include "Component/SceneComponent.h"
#include "Component/ColliderCircle.h"
#include "D2CharacterInfoComponent.h"
#include "D2DataManager.h"

CD2Projectile::CD2Projectile() :
	mInfo{}
{
	SetTypeID<CD2Projectile>();
}

CD2Projectile::CD2Projectile(const CD2Projectile& com)	:
	CObjectComponent(com)
{
	mInfo = com.mInfo;
}

CD2Projectile::~CD2Projectile()
{
}

bool CD2Projectile::Init()
{
	mRoot = mObject->GetRootSceneComponent();
	mCollider = mObject->FindSceneComponentFromType<CColliderCircle>();
	return true;
}

void CD2Projectile::Start()
{
	CObjectComponent::Start();

	if (!mRoot)
	{
		mRoot = mObject->GetRootSceneComponent();
	}
	if (!mCollider)
	{
		mCollider = mObject->FindSceneComponentFromType<CColliderCircle>();
	}
}

void CD2Projectile::Update(float deltaTime)
{
	mRoot->AddWorldPos(Vector3(mInfo.Dir.x, mInfo.Dir.y, 0.f) * mInfo.Speed);
}

void CD2Projectile::PostUpdate(float deltaTime)
{
}

void CD2Projectile::PrevRender()
{
}

void CD2Projectile::Render()
{
}

void CD2Projectile::PostRender()
{
}

CD2Projectile* CD2Projectile::Clone()
{
	return new CD2Projectile(*this);
}

void CD2Projectile::Save(FILE* fp)
{
	CComponent::Save(fp);

	int length = mInfo.Name.length();
	fwrite(&length, sizeof(int), 1, fp);
	fwrite(mInfo.Name.c_str(), sizeof(char), length, fp);
}

void CD2Projectile::Load(FILE* fp)
{
	CComponent::Load(fp);
	
	char name[128] = {};
	int length = 0;
	fread(&length, sizeof(int), 1, fp);
	fread(name, sizeof(char), length, fp);

	D2SkillInfo* info = CD2DataManager::GetInst()->FindSkillInfo(name);
	mInfo = *info;
}

void CD2Projectile::SetInfo(const std::string& infoName)
{
	D2SkillInfo* info = CD2DataManager::GetInst()->FindSkillInfo(infoName);
	mInfo = *info;
}

void CD2Projectile::OnCollideEnter(const CollisionResult& result)
{
 	CD2CharacterInfoComponent* com = result.Dest->GetGameObject()->FindObjectComponentFromType<CD2CharacterInfoComponent>();

	if (com)
	{
		com->SetHp(-mInfo.Damage);
	}

	mObject->Destroy();
}
