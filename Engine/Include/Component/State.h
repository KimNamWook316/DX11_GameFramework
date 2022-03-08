#pragma once

#include "../GameInfo.h"

class CState
{
public:
	CState();
	CState(const CState& state);
	virtual ~CState() = 0;

public:
	virtual bool Init() = 0;
	virtual void Start() = 0;
	CState* Update(float deltaTime);
	virtual void PostUpdate(float deltaTime) = 0;
	virtual CState* Clone() = 0;
	
	virtual void ResetState();

public:
	virtual void EnterStateFunction() = 0;
	virtual CState* StateFunction() = 0;
	virtual void ExitStateFunction() = 0;

public:
	bool IsEnd() const
	{
		return mbEnd;
	}

	void SetOwner(class CStateComponent* com)
	{
		mOwner = com;
	}

protected:
	class CStateComponent* mOwner;
	bool mbEnd;
	bool mbEnterState;
};

