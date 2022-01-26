#include "D2PlayerTownIdle.h"
#include "D2Player.h"
#include "D2PlayerRun.h"
#include "Component/SpriteComponent.h"

CD2PlayerTownIdle::CD2PlayerTownIdle()	:
	CD2State()
{
	mbIsLoop = true;
}

CD2PlayerTownIdle::~CD2PlayerTownIdle()
{
}

void CD2PlayerTownIdle::Update(float deltaTime)
{
}

void CD2PlayerTownIdle::OnEnterState(float deltaTime)
{
	mOwnerObject->GetSpriteComponent()->SetCurrentAnimation("TownIdle" + std::to_string((int)mOwnerObject->GetSpriteDir()));
}

void CD2PlayerTownIdle::OnExitState(float deltaTime)
{
}

void CD2PlayerTownIdle::Clear()
{
}

CD2State* CD2PlayerTownIdle::GetNextState()
{
	return nullptr;
}
