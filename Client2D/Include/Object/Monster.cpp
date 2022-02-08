#include "Monster.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "MonsterAnimation.h"
#include "../Widget/SimpleHUD.h"
#include "Engine.h"

CMonster::CMonster()
{
	SetTypeID<CMonster>();
}

CMonster::CMonster(const CMonster& obj)	:
	CGameObject(obj)
{
}

CMonster::~CMonster()
{
}

bool CMonster::Init()
{
	mSprite = CreateComponent<CSpriteComponent>("PlayerSprite");
	mBody = CreateComponent<CColliderCircle>("Body");
	mDissolve = CreateComponent<CDissolveComponent>("PaperBurn");

	mSimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");

	mSimpleHUD = mSimpleHUDWidget->CreateWidgetWindow<CSimpleHUD>("SimpleHUDWidget");

	SetRootSceneComponent(mSprite);

	mSprite->AddChild(mBody);
	mSprite->AddChild(mSimpleHUDWidget);

	mDissolve->SetMaterial(mSprite->GetMaterial());

	mSimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	mSimpleHUD->SetText(TEXT("¶ì¸ð¾Æ´Ô"));
	mSimpleHUD->SetHPPercent(0.5f);
	mSimpleHUD->SetHPDir(eProgressBarDir::BottomToTop);

	mBody->SetCollisionProfile("Monster");

	mSprite->SetTransparency(true);
	//mSprite->SetOpacity(0.5f);

	mSprite->CreateAnimationInstance<CMonsterAnimation>();

	mSprite->SetRelativeScale(100.f, 100.f, 1.f);
	mSprite->SetRelativePos(500.f, 300.f, 0.f);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);

	mBody->AddCollisionMouseCallBack(eCollisionState::Enter, this, &CMonster::OnCollsionMouseCallBack);
	mBody->AddCollisionMouseCallBack(eCollisionState::Exit, this, &CMonster::OnCollsionExitMouseCallBack);

	mBody->AddCollisionCallBack(eCollisionState::Enter, this, &CMonster::OnCollisionEnter);
	mBody->AddCollisionCallBack(eCollisionState::Exit, this, &CMonster::OnCollisionExit);

	mHP = 3;
	return true;
}

void CMonster::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CMonster::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::OnCollsionMouseCallBack(const CollisionResult& result)
{
	CEngine::GetInst()->SetMouseState(eMouseState::State1);
}

void CMonster::OnCollsionExitMouseCallBack(const CollisionResult& result)
{
	CEngine::GetInst()->SetMouseState(eMouseState::Normal);
}

void CMonster::OnCollisionEnter(const CollisionResult& result)
{
	--mHP;

	if (mHP <= 0)
	{
		mDissolve->StartDissolve();
		mBody->Enable(false);
	}
}

void CMonster::OnCollisionExit(const CollisionResult& result)
{
}

void CMonster::dissolveEnd()
{
	Destroy();
}
