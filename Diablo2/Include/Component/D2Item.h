#pragma once

#include "GameInfo.h"
#include "Ref.h"
#include "../D2Info.h"

class CD2Item	:
	public CRef
{
	friend class CD2ItemTable;
	friend class CD2Inventory;
	friend class CD2ItemObjectComponent;
	friend class CD2ChestObject;

protected:
	CD2Item();
	CD2Item(const CD2Item& item);
	virtual ~CD2Item();

public:
	virtual CD2Item* Clone();

public:
	void SetOwner(class CGameObject* obj);

public:
	const D2ItemInfo& GetItemInfo() const
	{
		return mInfo;
	}

	int GetCellWidth() const
	{
		return mInfo.CellX;
	}

	int GetCellHeight() const
	{
		return mInfo.CellY;
	}

	eD2ItemUseType GetItemUseType() const
	{
		return mInfo.eUseType;
	}

	eD2ItemType GetItemType() const
	{
		return mInfo.eType;
	}

	const std::string& GetName() const
	{
		return mInfo.Name;
	}

protected:
	class CGameObject* mOwner;
	class CD2CharacterInfoComponent* mCharInfo;
	D2ItemInfo mInfo;
};

