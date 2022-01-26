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
 //	// 가장 최근 Input Mouse Position 방향 으로 이동
 //	StateInputInfo info = mInputStack.top();
 //	CharacterInfo charInfo = mOwnerObject->GetCharInfo();
 //
 //	float remain = mOwnerObject->GetWorldPos().Distance(Vector3(info.InputPos.x, info.InputPos.y, 0.f));
 //
 //	if (remain < charInfo.CurMoveSpeed * deltaTime)
 //	{
 //		mbIsEnd = true;
 //	}
 //	else
 //	{
 //		Vector2 move(charInfo.Dir * charInfo.CurMoveSpeed * deltaTime);
 //		mOwnerObject->AddWorldPos(move.x, move.y, 0.f);
 //	}
}

void CD2PlayerRun::OnEnterState(float deltaTime)
{
	// SetMoveSpeed, dir
	mOwnerObject->GetSpriteComponent()->SetCurrentAnimation("Run" + std::to_string((int)mOwnerObject->GetSpriteDir()));
	mInputStack = static_cast<CD2Player*>(mOwnerObject)->mInputStack;
}

void CD2PlayerRun::OnExitState(float deltaTime)
{
	// SetMoveSpeed, dir
}

void CD2PlayerRun::Clear()
{
}

CD2State* CD2PlayerRun::GetNextState()
{
	MouseInputInfo info = mInputStack->GetMouseInputTop();

	return nullptr;
}
