#include "PlayerWalkState.h"
#include "PlayerRunState.h"
#include "PlayerIdleState.h"
#include "../Component/D2CharacterInfoComponent.h"
#include "Component/StateComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/NavAgentComponent.h"
#include "../Component/D2StateComponent.h"
#include "../Component/D2CharacterInfoComponent.h"
#include "GameObject/GameObject.h"
#include "Input.h"
#include "../D2Util.h"

CPlayerWalkState::CPlayerWalkState()	:
	mbRun(false),
	mbWalkEnd(false),
	mPrevDir(eD2SpriteDir::S)
{
}

CPlayerWalkState::CPlayerWalkState(const CPlayerWalkState& state)	:
	CState(state)
{
	mbRun = false;
	mbWalkEnd = false;
	mPrevDir = eD2SpriteDir::S;
}

CPlayerWalkState::~CPlayerWalkState()
{
}

bool CPlayerWalkState::Init()
{
	return true;
}

void CPlayerWalkState::Start()
{
}

void CPlayerWalkState::PostUpdate(float deltaTime)
{
}

CPlayerWalkState* CPlayerWalkState::Clone()
{
	return new CPlayerWalkState(*this);
}

void CPlayerWalkState::EnterStateFunction()
{
	CInput::GetInst()->SetKeyCallBack("LCtrl", eKeyState::KeyState_Down, this, &CPlayerWalkState::OnCtrlDown);
	CInput::GetInst()->SetKeyCallBack("MouseLCtrl", eKeyState::KeyState_Push, this, &CPlayerWalkState::OnPushMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseL", eKeyState::KeyState_Push, this, &CPlayerWalkState::OnPushMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseR", eKeyState::KeyState_Push, this, &CPlayerWalkState::OnPushMouseR);

	mPrevDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation("Walk" + std::to_string((int)mPrevDir));

	mOwner->GetNavAgent()->SetFrameCallBack(this, &CPlayerWalkState::OnWalk);
	mOwner->GetNavAgent()->SetEndCallBack(this, &CPlayerWalkState::OnWalkEnd);
}

CState* CPlayerWalkState::StateFunction()
{
	if (mbRun)
	{
		mbEnd = true;
		mOwner->GetNavAgent()->SetMoveSpeed(static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetMaxSpeed());
		return (CState*)(new CPlayerRunState);
	}
	return nullptr;
}

void CPlayerWalkState::ExitStateFunction()
{
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("MouseLCtrl", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseL", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseR", eKeyState::KeyState_Push);
	mOwner->GetNavAgent()->DeleteFrameCallBack();
	mOwner->GetNavAgent()->DeleteEndCallBack();
}

void CPlayerWalkState::ResetState()
{
	CState::ResetState();
	mbWalkEnd = false;
	mbRun = false;
}

void CPlayerWalkState::OnPushMouseR(float deltaTime)
{
}

void CPlayerWalkState::OnPushMouseL(float deltaTime)
{
}

void CPlayerWalkState::OnCtrlDown(float deltaTime)
{
	mbRun = true;
}

void CPlayerWalkState::OnWalkEnd()
{
	mbEnd = true;
	mbWalkEnd = true;
}

void CPlayerWalkState::OnWalk()
{
	Vector3 direction = mOwner->GetNavAgent()->GetPathListFront() - mOwner->GetRootComponent()->GetWorldPos();
	direction.Normalize();
	static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->SetDir(Vector2(direction.x, direction.y));

	eD2SpriteDir spriteDir = static_cast<CD2StateComponent*>(mOwner)->GetCharInfo()->GetSpriteDir();
	if (mPrevDir != spriteDir)
	{
		mPrevDir = spriteDir;
		static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation("Walk" + std::to_string((int)mPrevDir));
	}
}
