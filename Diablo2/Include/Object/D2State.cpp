#include "D2State.h"
#include "D2Object.h"

CD2State::CD2State()	:
	mOwnerObject(nullptr),
	mbIsLoop(false)
{
}

CD2State::CD2State()
{
}

CD2State::CD2State(const CD2State& state)	:
	mOwnerObject(nullptr),
	mbIsLoop(false)
{
	*this = state;
}

CD2State::~CD2State()
{
}
