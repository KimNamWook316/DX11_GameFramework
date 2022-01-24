#include "D2PlayerTownIdle.h"
#include "D2Player.h"
#include "D2PlayerRun.h"

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
}

void CD2PlayerTownIdle::OnExitState(float deltaTime)
{
}

void CD2PlayerTownIdle::Clear()
{
}

CD2State* CD2PlayerTownIdle::GetNextState()
{
	if (!mInputQueue.empty())
	{
		D2StateInfo::InputInfo info = mInputQueue.top();
		if (eInputState::MouseLButton == (eInputState)info.eInputState)
		{
		}
		else if (eInputState::MouseRButton == (eInputState)info.eInputState)
		{
			return (CD2State*)(new CD2PlayerRun);
		}
	}
	else
	{
		return nullptr;
	}
}
