#include "D2State.h"

CD2State::CD2State()	:
	mbIsHit(false),
	mbDie(false)
{
}

CD2State::CD2State(const CState& state)	:
	CState(state)
{
	mbIsHit = false;
	mbDie = false;
}

CD2State::~CD2State()
{
}

void CD2State::ResetState()
{
	mbIsHit = false;
}

void CD2State::OnDie()
{
	mbDie = true;
}

void CD2State::OnCollideEnter(const CollisionResult& result)
{
}
