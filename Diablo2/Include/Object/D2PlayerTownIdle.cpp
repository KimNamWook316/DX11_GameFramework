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
	mOwnerObject->SetMoveSpeed(0.f);
	mInputStack = static_cast<CD2Player*>(mOwnerObject)->GetInputStackComponent();
}

void CD2PlayerTownIdle::OnExitState(float deltaTime)
{
}

void CD2PlayerTownIdle::Clear()
{
}

CD2State* CD2PlayerTownIdle::GetNextState()
{
	if (!mInputStack->IsMouseInputEmpty())
	{
		MouseInputInfo info = mInputStack->GetMouseInputTop();
		if (eMouseClickState::MouseRButton == info.State)
		{
			return (CD2State*)(new CD2PlayerRun);
		}
		else
		{
			return nullptr;
		}
	}
	return nullptr;
}
