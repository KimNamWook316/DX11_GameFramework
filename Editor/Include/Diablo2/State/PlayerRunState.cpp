#include "PlayerRunState.h"
#include "Component/SpriteComponent.h"
#include "Component/NavAgentComponent.h"
#include "Component/StateComponent.h"
#include "../Component/D2CharacterInfoComponent.h"
#include "PlayerWalkState.h"
#include "GameObject/GameObject.h"
#include "Input.h"
#include "../D2Util.h"

CPlayerRunState::CPlayerRunState()	:
	mbRunEnd(false),
	mPrevDir(eD2SpriteDir::S)
{
}

CPlayerRunState::CPlayerRunState(const CPlayerRunState& state)	:
	CState(state)
{
}

CPlayerRunState::~CPlayerRunState()
{
}

bool CPlayerRunState::Init()
{
	return true;
}

void CPlayerRunState::Start()
{
	mCharInfo = mOwner->GetGameObject()->FindObjectComponentFromType<CD2CharacterInfoComponent>();
}

void CPlayerRunState::PostUpdate(float deltaTime)
{
}

CState* CPlayerRunState::Clone()
{
	return new CPlayerRunState(*this);
}

void CPlayerRunState::EnterStateFunction()
{
	CInput::GetInst()->SetKeyCallBack("LCtrl", eKeyState::KeyState_Up, this, &CPlayerRunState::OnCtrlUp);
	CInput::GetInst()->SetKeyCallBack("MouseLCtrl", eKeyState::KeyState_Push, this, &CPlayerRunState::OnPushMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseL", eKeyState::KeyState_Push, this, &CPlayerRunState::OnPushMouseL);
	CInput::GetInst()->SetKeyCallBack("MouseR", eKeyState::KeyState_Push, this, &CPlayerRunState::OnPushMouseR);

	mPrevDir = mCharInfo->GetSpriteDir();
	static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation("Run" + std::to_string((int)mPrevDir));

	mOwner->GetNavAgent()->SetFrameCallBack(this, &CPlayerRunState::OnRun);
	mOwner->GetNavAgent()->SetEndCallBack(this, &CPlayerRunState::OnRunEnd);
}

CState* CPlayerRunState::StateFunction()
{
	if (mbRunEnd)
	{
		mbEnd = true;
		return nullptr;
	}
	else if (mbWalk)
	{
		mbEnd = true;
		mOwner->GetNavAgent()->SetMoveSpeed(mCharInfo->GetMaxSpeed() / 2.f);
		return (CState*)(new CPlayerWalkState);
	}
	return nullptr;
}

void CPlayerRunState::ExitStateFunction()
{
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Down);
	CInput::GetInst()->DeleteCallBack("LCtrl", eKeyState::KeyState_Up);
	CInput::GetInst()->DeleteCallBack("MouseLCtrl", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseL", eKeyState::KeyState_Push);
	CInput::GetInst()->DeleteCallBack("MouseR", eKeyState::KeyState_Push);
	mOwner->GetNavAgent()->DeleteFrameCallBack();
	mOwner->GetNavAgent()->DeleteEndCallBack();
}

void CPlayerRunState::ResetState()
{
	CState::ResetState();
	mbRunEnd = false;
	mbWalk = false;
}

void CPlayerRunState::OnPushMouseR(float deltaTime)
{
}

void CPlayerRunState::OnPushMouseL(float deltaTime)
{
}

void CPlayerRunState::OnCtrlUp(float delatTime)
{
	mbWalk = true;
}

void CPlayerRunState::OnRunEnd()
{
	mbRunEnd = true;
}

void CPlayerRunState::OnRun()
{
	Vector3 direction = mOwner->GetNavAgent()->GetPathListFront() - mOwner->GetRootComponent()->GetWorldPos();
	direction.Normalize();
	mCharInfo->SetDir(Vector2(direction.x, direction.y));

	eD2SpriteDir spriteDir = mCharInfo->GetSpriteDir();
	if (mPrevDir != spriteDir)
	{
		mPrevDir = spriteDir;
		static_cast<CSpriteComponent*>(mOwner->GetRootComponent())->SetCurrentAnimation("Run" + std::to_string((int)mPrevDir));
	}
}
