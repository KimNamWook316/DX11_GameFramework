#pragma once

#include "../GameInfo.h"

// ������ �����ϴ� �Ŵ���
class CRenderManager
{
public:
	void SetObjList(const std::list<CSharedPtr<class CGameObject>>* pList)
	{
		mpObjList = pList;
	}
	void AddRenderList(class CSceneComponent* pComponent);

public:
	bool Init();
	void Render();
	DECLARE_SINGLE(CRenderManager)

private:
	int mRenderCount;
	std::vector<class CSceneComponent*> mVecRender;
	const std::list<CSharedPtr<class CGameObject>>* mpObjList;
};

