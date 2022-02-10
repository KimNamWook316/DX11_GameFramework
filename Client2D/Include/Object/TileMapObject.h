#pragma once

#include "GameObject/GameObject.h"
#include "Component/TileMapComponent.h"

class CTileMapObject :
    public CGameObject
{
    friend class CScene;

protected:
    CTileMapObject();
    CTileMapObject(const CTileMapObject& obj);
    virtual ~CTileMapObject();

public:
	virtual bool Init();
    virtual void Start();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CTileMapObject* Clone();

private:
    CSharedPtr<CTileMapComponent> mTileMap;
};

