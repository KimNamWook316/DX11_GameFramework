#include "GameObject.h"

CGameObject::CGameObject()	:
	mScene(nullptr)
{
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject& obj)
{
	*this = obj;
	mRefCount = 0;
}

CGameObject::~CGameObject()
{
}

void CGameObject::SetScene(CScene* scene)
{
	mScene = scene;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float deltaTime)
{
}

void CGameObject::PostUpdate(float deltaTime)
{
}

void CGameObject::PrevRender()
{
}

void CGameObject::Render()
{
}

void CGameObject::PostRender()
{
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}
