#pragma once

#include "GameInfo.h"
#include "../D2Info.h"
#include "D2Item.h"

enum class eItemTableCSVLabel
{
	Name,
	UniqueID,
	Type,
	CellXWidth,
	CellYWidth,
	UseType,
	Hp,
	Mp,
	Melee,
	Magic,
	UITexturePath,
	ObjTexturePath,
	ObjAnimSequencePath
};

class CD2ItemTable
{
	friend class CD2ItemTableComponent;

public:
	virtual bool Init();
	virtual void Start();

public:
	CD2Item* GenerateItem(const std::string& name);
	CD2Item* GenerateRandomItem();
	class CTexture* FindItemUITexture(CD2Item* item);
	class CTexture* FindItemObjTexture(CD2Item* item);
	const std::string& FindItemObjAnimSequenceName(CD2Item* item);

private:
	bool loadTable();

	DECLARE_SINGLE(CD2ItemTable)

private:
	std::unordered_map<std::string, D2ItemInfo*> mMapItemInfo;
	std::unordered_map<std::string, std::string> mMapObjAnimSequnceName;
};

