#include "TileMapObject.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "Engine.h"

CTileMapObject::CTileMapObject()
{
 //	SetTypeID<CTileMapObject>();
}

CTileMapObject::CTileMapObject(const CTileMapObject& obj)	:
	CGameObject(obj)
{
 //	mTileMap = (CTileMapComponent*)FindComponent("TileMap");
}

CTileMapObject::~CTileMapObject()
{
}

bool CTileMapObject::Init()
{
 //	mTileMap = CreateComponent<CTileMapComponent>("TileMap");
 //	SetRootSceneComponent(mTileMap);
 //
 //	mTileMap->CreateTile(eTileShape::Rhombus, 100, 100, Vector2(160.f, 80.f));
 //
 //	CMaterial* mat = mScene->GetResource()->FindMaterial("TileMap");
 //	mTileMap->SetTileMaterial(mat);
 //	mTileMap->SetTileDefaultFrame(0.f, 0.f, 160.f, 80.f);
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
