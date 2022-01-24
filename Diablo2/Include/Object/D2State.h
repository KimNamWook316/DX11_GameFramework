#pragma once

#include "../Dia2Info.h"
#include "GameInfo.h"

namespace D2StateInfo
{
	struct InputInfo
	{
		int Priority;
		int eInputState;

		InputInfo() :
			Priority(0),
			eInputState(-1),
		{
		}

		bool operator < (const InputInfo& info)
		{
			return Priority > info.Priority;
		}
	};
}

class CD2State
{
public:
	CD2State();
	CD2State(const CD2State& state);
	virtual ~CD2State() = 0;

public:
	virtual void Update(float deltaTime) = 0;
	virtual void OnEnterState(float deltaTime) = 0;
	virtual void OnExitState(float deltaTime) = 0;
	virtual void Clear() = 0;
	virtual CD2State* GetNextState() = 0;

public:
	void PushInput(const int priority, int eInput)
	{
		D2StateInfo::InputInfo info;
		info.Priority = priority;
		info.eInputState = eInput;
		mInputQueue.push(info);
	}

protected:
	class CD2Object* mOwnerObject;
	std::priority_queue<D2StateInfo::InputInfo> mInputQueue;
	bool mbIsLoop;
};

