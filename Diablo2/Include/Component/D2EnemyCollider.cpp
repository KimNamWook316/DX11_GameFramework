#include "D2EnemyCollider.h"
#include "Component/ColliderBox2D.h"
#include "Collision/Collision.h"
#include "D2CharacterInfoComponent.h"
#include "GameObject/GameObject.h"
#include "D2StateComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneCollision.h"
#include "D2UIManager.h"

CD2EnemyCollider::CD2EnemyCollider()
{
	SetTypeID<CD2EnemyCollider>();
}

CD2EnemyCollider::CD2EnemyCollider(const CD2EnemyCollider& com)	:
	CObjectComponent(com)
{
}

CD2EnemyCollider::~CD2EnemyCollider()
{
	if (mScene->GetCollision())
	{
		mScene->GetCollision()->DeleteCollider(mCollider);
	}
}

bool CD2EnemyCollider::Init()
{
	return true;
}

void CD2EnemyCollider::Start()
{
	CObjectComponent::Start();

	mCollider = mObject->FindSceneComponentFromType<CColliderBox2D>();
	mState = mObject->FindObjectComponentFromType<CD2StateComponent>();
	mCharInfo = mObject->FindObjectComponentFromType<CD2CharacterInfoComponent>();

	mCollider->AddCollisionCallBack(eCollisionState::Enter, this, &CD2EnemyCollider::OnCollideEnter);
	mCollider->AddCollisionCallBack(eCollisionState::Exit, this, &CD2EnemyCollider::OnCollideExit);
	mCollider->AddCollisionMouseCallBack(eCollisionState::Enter, this, &CD2EnemyCollider::OnMouseCollideEnter);
	mCollider->AddCollisionMouseCallBack(eCollisionState::Exit, this, &CD2EnemyCollider::OnMouseCollideExit);
	mCharInfo->SetEventCallBack("OnHpChange", this, &CD2EnemyCollider::OnHpChange, eD2CharInfoEventType::HpDec);
	mCharInfo->SetEventCallBack("OnHpChange", this, &CD2EnemyCollider::OnHpChange, eD2CharInfoEventType::HpInc);
}

void CD2EnemyCollider::Update(float deltaTime)
{
}

void CD2EnemyCollider::PostUpdate(float deltaTime)
{
}

void CD2EnemyCollider::PrevRender()
{
}

void CD2EnemyCollider::Render()
{
}

void CD2EnemyCollider::PostRender()
{
}

CD2EnemyCollider* CD2EnemyCollider::Clone()
{
	return new CD2EnemyCollider(*this);
}

void CD2EnemyCollider::OnCollideEnter(const CollisionResult& result)
{
	CollisionProfile* profile = result.Dest->GetCollisionProfile();

	// 플레이어와 충돌한 경우, 공격상태로 전환
	// 플레이어 공격에 맞은 경우, 피격 애니메이션 전환
	// 몬스터끼리 충돌한 경우, 움직이는 중이라면 길찾기 도중에 충돌한 것이므로, wait 상태로 변경
	// 충돌체에서 위치 보정하고, State에서 처리
	switch (profile->Channel)
	{
	case eCollisionChannel::Player:
	{
		//adjustPosition(result);
		mState->OnCollideEnter(result);
		break;
	}
	case eCollisionChannel::PlayerAttack:
	{
		// 애니메이션 처리
		mState->OnCollideEnter(result);
		break;
	}
	case eCollisionChannel::Monster:
	{
		// 몬스터끼리 충돌 처리
		adjustPosition(result);
		mState->OnCollideEnter(result);
	}
	}
}

void CD2EnemyCollider::OnCollideExit(const CollisionResult& result)
{
 //	CollisionProfile* profile = result.Dest->GetCollisionProfile();
 //
 //	switch (profile->Channel)
 //	{
 //	case eCollisionChannel::Monster:
 //	{
 //		// 몬스터끼리 충돌 처리
 //		//mState->OnCollideExit(result);
 //	}
 //	}
}

void CD2EnemyCollider::OnMouseCollideEnter(const CollisionResult& result)
{
	mbMouseCollide = true;
	CD2UIManager::GetInst()->EnableMonsterStateWindow(true);
	CD2UIManager::GetInst()->UpdateMonsterStateWindow(mCharInfo->GetInfo().Name, mCharInfo->GetHpPercent());
}

void CD2EnemyCollider::OnMouseCollideExit(const CollisionResult& result)
{
	mbMouseCollide = false;
	CD2UIManager::GetInst()->EnableMonsterStateWindow(false);
}

void CD2EnemyCollider::OnHpChange()
{
	if (mbMouseCollide)
	{
		CD2UIManager::GetInst()->UpdateMonsterStateWindow(mCharInfo->GetInfo().Name, mCharInfo->GetHpPercent());
	}
}

void CD2EnemyCollider::adjustPosition(const CollisionResult& result)
{
	// 충돌 위치 보정
	Box2DInfo srcInfo = static_cast<CColliderBox2D*>(result.Src)->GetInfo();
	Box2DInfo destInfo = static_cast<CColliderBox2D*>(result.Dest)->GetInfo();
	Box2DInfo overLap = CCollision::GetOverlapAreaAABB(srcInfo, destInfo);

	Vector3 colDir = result.Dest->GetWorldPos() - result.Src->GetWorldPos();

	Vector2 moveAmount;

	if (overLap.Length.x > overLap.Length.y)
	{
		if (colDir.y > 0)
		{
			moveAmount.y = -overLap.Length.y * 2.f;
		}
		else
		{
			moveAmount.y = overLap.Length.y * 2.f;
		}
	}
	else
	{
		if (colDir.x > 0)
		{
			moveAmount.x = -overLap.Length.x * 2.f;
		}
		else
		{
			moveAmount.x = overLap.Length.x * 2.f;
		}
	}

	mObject->AddWorldPos(moveAmount.x, moveAmount.y, 0.f);
}
