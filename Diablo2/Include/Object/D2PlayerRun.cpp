#include "D2PlayerRun.h"
#include "D2PlayerTownIdle.h"
#include "Component/SpriteComponent.h"
#include "../Component/InputStackComponent.h"
#include "D2State.h"
#include "D2Player.h"

CD2PlayerRun::CD2PlayerRun() :
	CD2State(),
	mInputStack(nullptr)
{
	mbIsLoop = false;
}

CD2PlayerRun::~CD2PlayerRun()
{
}

void CD2PlayerRun::Update(float deltaTime)
{
	// 가장 최근 Input Mouse Position 방향 으로 이동
	if (!mInputStack->IsMouseInputEmpty())
	{
		MouseInputInfo info = mInputStack->GetMouseInputTop();

		if (eMouseClickState::MouseRButton == info.State)
		{
			mTargetPos = info.Position;
			Vector3 dir = Vector3(mTargetPos.x, mTargetPos.y, 0.f) - mOwnerObject->GetWorldPos();
			dir.Normalize();
			eSpriteDir prevSpriteDir = mOwnerObject->GetSpriteDir();
			mOwnerObject->SetDirection(dir);
			
			if (prevSpriteDir != mOwnerObject->GetSpriteDir())
			{
				mOwnerObject->GetSpriteComponent()->SetCurrentAnimation("Run" + std::to_string((int)mOwnerObject->GetSpriteDir()));
			}
			mInputStack->ClearMouseInput();
		}
	}
	CharacterInfo charInfo = mOwnerObject->GetCharInfo();

	float remain = mOwnerObject->GetWorldPos().Distance(Vector3(mTargetPos.x, mTargetPos.y, 0.f));

	if (remain < charInfo.CurMoveSpeed * deltaTime)
	{
		mbIsEnd = true;
	}
	else
	{
		Vector2 move(charInfo.Dir * charInfo.CurMoveSpeed * deltaTime);
		mOwnerObject->AddWorldPos(move.x, move.y, 0.f);
	}
}

void CD2PlayerRun::OnEnterState(float deltaTime)
{
	mInputStack = static_cast<CD2Player*>(mOwnerObject)->GetInputStackComponent();
	// SetMoveSpeed, dir
	MouseInputInfo info = mInputStack->GetMouseInputTop();

	if (eMouseClickState::MouseRButton == info.State)
	{
		mTargetPos = info.Position;
		Vector3 dir = Vector3(mTargetPos.x, mTargetPos.y, 0.f) - mOwnerObject->GetWorldPos();
		dir.Normalize();
		mOwnerObject->SetDirection(dir);
	}

	mOwnerObject->GetSpriteComponent()->SetCurrentAnimation("Run" + std::to_string((int)mOwnerObject->GetSpriteDir()));
	mOwnerObject->SetMoveSpeed(mOwnerObject->GetMaxMoveSpeed());
}

void CD2PlayerRun::OnExitState(float deltaTime)
{
	mOwnerObject->SetMoveSpeed(0.f);
}

void CD2PlayerRun::Clear()
{
}

CD2State* CD2PlayerRun::GetNextState()
{
	return nullptr;
}
