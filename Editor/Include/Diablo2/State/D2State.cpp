#include "D2State.h"

CD2State::CD2State()	:
	mbIsHit(false)
{
}

CD2State::CD2State(const CState& state)	:
	CState(state)
{
}

CD2State::~CD2State()
{
}

void CD2State::ResetState()
{
	mbIsHit = false;
}
