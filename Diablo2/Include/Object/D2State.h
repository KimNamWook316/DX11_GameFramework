#pragma once

#include "../Dia2Info.h"
#include "GameInfo.h"

class CD2State
{
	friend class CD2Object;

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

protected:
	class CD2Object* mOwnerObject;
	bool mbIsLoop;
	bool mbIsEnd;
};

