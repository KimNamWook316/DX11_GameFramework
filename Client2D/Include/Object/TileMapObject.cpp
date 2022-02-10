#include "TileMapObject.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "Engine.h"

CTileMapObject::CTileMapObject()
{
	SetTypeID<CTileMapObject>();
}

CTileMapObject::CTileMapObject(const CTileMapObject& obj)	:
	CGameObject(obj)
{
	mTileMap = (CTileMapComponent*)FindComponent("TileMap");
}

CTileMapObject::~CTileMapObject()
{
}

bool CTileMapObject::Init()
{
	mTileMap = CreateComponent<CTileMapComponent>("TileMap");
	SetRootSceneComponent(mTileMap);

	mTileMap->CreateTile(eTileShape::Rect, 100, 100, Vector2(64.f, 64.f));

	CMaterial* mat = mScene->GetResource()->FindMaterial("TileMap");
	mTileMap->SetTileMaterial(mat);
	return true;
}

void CTileMapObject::Start()
{
	CGameObject::Start();
}

void CTileMapObject::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CTileMapObject::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CTileMapObject* CTileMapObject::Clone()
{
	return new CTileMapObject(*this);
}
