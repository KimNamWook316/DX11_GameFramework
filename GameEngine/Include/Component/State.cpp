#include "State.h"

CState::CState() :
	mbEnterState(false),
	mbEnd(false),
	mOwner(nullptr)
{
}

CState::CState(const CState& state)	:
	mbEnterState(false)
{
	mOwner = nullptr;
}


CState::~CState()
{
}

CState* CState::Update(float deltaTime)
{

	return StateFunction();
}

void CState::ResetState()
{
}
