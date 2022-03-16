#pragma once

#include "Component/State.h"

class CD2State :
    public CState
{
public:
	CD2State();
	CD2State(const CState& state);
	virtual ~CD2State();

public:
	virtual void ResetState() override;

public:
	virtual void OnCollideEnter(const CollisionResult& result) = 0;
	virtual void OnCollideExit(const CollisionResult& result) = 0;

protected:
	bool mbIsHit;
};

