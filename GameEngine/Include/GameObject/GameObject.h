#pragma once

#include "../GameInfo.h"
#include "../Ref.h"

class CGameObject : public CRef
{
	friend class CScene;

protected:
	CGameObject();
	virtual ~CGameObject();

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render();
};

