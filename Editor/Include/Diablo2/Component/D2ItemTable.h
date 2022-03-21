#pragma once

#include "GameInfo.h"
#include "../D2Info.h"
#include "D2Item.h"

enum eCSVLabel
{
	Name,
	UniqueID,
	CellXWidth,
	CellYWidth,
	UseType,
	Hp,
	Mp,
	Melee,
	Magic,
	UITexturePath,
	ObjTexturePath,
};

class CD2ItemTable
{
	friend class CD2ItemTableComponent;

public:
	virtual bool Init();
	virtual void Start();

public:
	CD2Item* GenerateItem(const std::string& name);

private:
	bool loadTable();

	DECLARE_SINGLE(CD2ItemTable)

private:
	std::unordered_map<std::string, D2ItemInfo*> mMapItemInfo;
};

