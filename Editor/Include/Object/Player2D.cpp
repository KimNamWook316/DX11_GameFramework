#include "Player2D.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Animation/AnimationSequence2DInstance.h"

CPlayer2D::CPlayer2D()
{
	SetTypeID<CPlayer2D>();
}

CPlayer2D::CPlayer2D(const CPlayer2D& obj) :
	CGameObject(obj)
{
	mSprite = (CSpriteComponent*)FindComponent("PlayerSprite");
}

CPlayer2D::~CPlayer2D()
{
}

void CPlayer2D::Start()
{
	CGameObject::Start();
}

bool CPlayer2D::Init()
{
	mSprite = CreateComponent<CSpriteComponent>("PlayerSprite");

	SetRootSceneComponent(mSprite);
	mSprite->SetTransparency(true);

	mSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	CAnimationSequence2DInstance* anim = mSprite->GetAnimationInstance();

	anim->AddAnimation(TEXT("one.sqc"), ANIMATION_PATH, "Idle", true);
	anim->AddAnimation(TEXT("two.sqc"), ANIMATION_PATH, "Run", true);

	mSprite->SetRelativeScale(100.f, 100.f, 1.f);
	mSprite->SetRelativePos(100.f, 50.f, 0.f);
	mSprite->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CPlayer2D::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPlayer2D* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CPlayer2D::Load(FILE* File)
{
	CGameObject::Load(File);

	mSprite = (CSpriteComponent*)FindComponent("PlayerSprite");
}
