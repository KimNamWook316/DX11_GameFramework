#include "D2SkillObject.h"
#include "GameObject/GameObject.h"
#include "Component/ColliderComponent.h"
#include "D2DataManager.h"
#include "../D2Util.h"
#include "Scene/Scene.h"

CD2SkillObject::CD2SkillObject() :
	mSkillOwner(nullptr),
	mTargetObj(nullptr),
	mInfo{},
	mbEditMode(false)
{
	SetTypeID<CD2SkillObject>();
}

CD2SkillObject::CD2SkillObject(const CD2SkillObject& com)	:
	CObjectComponent(com)
{
	mInfo = com.mInfo;
	mSkillOwner = com.mSkillOwner;
	mTargetObj = com.mTargetObj;
	mTargetPos = com.mTargetPos;
}

CD2SkillObject::~CD2SkillObject()
{
}

bool CD2SkillObject::Init()
{
	return true;
}

void CD2SkillObject::Start()
{
	mRoot = mObject->GetRootSceneComponent();
}

void CD2SkillObject::Update(float deltaTime)
{
	if (!mbEditMode)
	{
		if (mRoot)
		{
			mRoot->AddWorldPos(Vector3(mInfo.Dir.x, mInfo.Dir.y, 0.f) * mInfo.Speed * deltaTime);
		}
	}
}

void CD2SkillObject::PostUpdate(float deltaTime)
{
}

void CD2SkillObject::PrevRender()
{
}

void CD2SkillObject::Render()
{
}

void CD2SkillObject::PostRender()
{
}

void CD2SkillObject::Save(FILE* fp)
{
	CComponent::Save(fp);
}

void CD2SkillObject::Load(FILE* fp)
{
	CComponent::Load(fp);
}

void CD2SkillObject::SetInfo(const std::string& name)
{
	D2SkillInfo* info = CD2DataManager::GetInst()->FindSkillInfo(name);
	mInfo = *info;
}

void CD2SkillObject::SetDir(const Vector2& dir)
{
	mInfo.Dir = dir;
	mInfo.Speed = CD2Util::CorrectSpeed(mInfo.Dir, mInfo.Speed);
}

