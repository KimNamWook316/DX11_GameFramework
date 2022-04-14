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
	virtual CD2StateComponent* Clone() override;

public:
	void OnDie();

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

	class CD2ShadowComponent* GetShadow() const
	{
		return mShadow;
	}

	class CColliderBox2D* GetCollider() const
	{
		return mCollider;
	}

	class CD2ClickableObject* GetClickableObject() const
	{
		return mClickableObject;
	}

	void SaveMousePos(const Vector2& mousePos)
	{
		mPrevStateMousePos = mousePos;
	}

	void ReserveSkillTargetPos(const Vector3& pos)
	{
		mReserveSkillTargetPos = pos;
	}

	const Vector2& GetPrevStateMousePos() const
	{
		return mPrevStateMousePos;
	}

	const Vector3& GetReserveSkillTargetPos() const
	{
		return mReserveSkillTargetPos;
	}

	void SetClickableObject(class CD2ClickableObject* clickObj)
	{
		mClickableObject = clickObj;
	}

protected:
	CD2CharacterInfoComponent* mCharInfo;
	CD2PlayerSkillComponent* mSkill;
	CD2EnemySkillComponent* mEnemySkill;
	class CD2ShadowComponent* mShadow;
	class CColliderBox2D* mCollider;
	Vector2 mPrevStateMousePos;
	Vector3 mReserveSkillTargetPos;

	// Clickable Object
	// 주워야 할 아이템과 마우스가 충돌했고, 주울 수 있는 범위 안일 경우 들어옴
	class CD2ClickableObject* mClickableObject;
};

