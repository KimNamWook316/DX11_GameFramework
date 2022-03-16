#pragma once

#include "Component/StateComponent.h"
#include "D2CharacterInfoComponent.h"
#include "D2PlayerSkillComponent.h"
#include "D2EnemySkillComponent.h"

class CD2StateComponent :
    public CStateComponent
{
	friend class CGameObject;

protected:
	CD2StateComponent();
	CD2StateComponent(const CD2StateComponent& com);
	virtual ~CD2StateComponent();

public:
	virtual void Start() override;

public:
	void OnCollideEnter(const CollisionResult& result);
	void OnCollideExit(const CollisionResult& result);

public:
	CD2CharacterInfoComponent* GetCharInfo() const
	{
		return mCharInfo;
	}

	CD2PlayerSkillComponent* GetSkill() const
	{
		return mSkill;
	}

	CD2EnemySkillComponent* GetEnemySkill() const
	{
		return mEnemySkill;
	}

	class CColliderBox2D* GetCollider() const
	{
		return mCollider;
	}

	void SaveMousePos(const Vector2& mousePos)
	{
		mPrevStateMousePos = mousePos;
	}

	const Vector2& GetPrevStateMousePos() const
	{
		return mPrevStateMousePos;
	}

protected:
	CSharedPtr<CD2CharacterInfoComponent> mCharInfo;
	CSharedPtr<CD2PlayerSkillComponent> mSkill;
	CSharedPtr<CD2EnemySkillComponent> mEnemySkill;
	CSharedPtr<class CColliderBox2D> mCollider;
	Vector2 mPrevStateMousePos;
};

