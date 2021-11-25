#pragma once
#include "../Ref.h"
class CSceneMode : public CRef
{
	friend class CScene;

protected:
	CSceneMode();
	virtual ~CSceneMode();

public:
	void SetPlayerObj(class CGameObject* obj);

	class CGameObject* GetPlayerObj()	const
	{
		return mPlayerObj;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);

protected:
	class CScene* mScene;
	CSharedPtr<class CGameObject> mPlayerObj;
};

