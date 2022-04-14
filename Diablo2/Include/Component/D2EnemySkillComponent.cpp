#include "D2EnemySkillComponent.h"
#include "GameObject/GameObject.h"
#include "Resource/ResourceManager.h"
#include "Scene/Scene.h"
#include "../D2Util.h"
#include "D2ObjectPool.h"
#include "D2SkillObject.h"

CD2EnemySkillComponent::CD2EnemySkillComponent()
{
	SetTypeID<CD2EnemySkillComponent>();
}

CD2EnemySkillComponent::CD2EnemySkillComponent(const CD2EnemySkillComponent& com)	:
	CObjectComponent(com)
{
}

CD2EnemySkillComponent::~CD2EnemySkillComponent()
{
}

bool CD2EnemySkillComponent::Init()
{
	return true;
}

void CD2EnemySkillComponent::Start()
{
	CObjectComponent::Start();
}

void CD2EnemySkillComponent::Update(float deltaTime)
{
}

void CD2EnemySkillComponent::PostUpdate(float deltaTime)
{
}

void CD2EnemySkillComponent::PrevRender()
{
}

void CD2EnemySkillComponent::Render()
{
}

void CD2EnemySkillComponent::PostRender()
{
}

CD2EnemySkillComponent* CD2EnemySkillComponent::Clone()
{
	return new CD2EnemySkillComponent(*this);
}

CGameObject* CD2EnemySkillComponent::DoSkill(const std::string& name, const Vector3& startPos, const Vector3& targetPos, const Vector2& dir, CGameObject* targetObj)
{
	CGameObject* obj = CD2ObjectPool::GetInst()->CloneSkillObj(name);

	if (!obj)
	{
		assert(false);
		return nullptr;
	}

	obj->Start();
	obj->Enable(true);
	obj->SetWorldPos(startPos);

	CD2SkillObject* objController = (CD2SkillObject*)obj->FindComponent("Script");
	
	float damage = objController->GetDamage();
	float additional = damage;

	objController->SetDamage(damage + additional);
	objController->SetDir(dir);
	objController->SetStartPos(startPos);
	objController->SetTargetPos(targetPos);
	objController->SetSkillOwner(mObject);
	objController->SetTargetObject(targetObj);

	return obj;
}
