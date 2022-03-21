#pragma once

#include "GameInfo.h"
#include "../D2Info.h"

class CD2Item
{
	friend class CD2ItemTable;

protected:
	CD2Item();
	virtual ~CD2Item();

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

protected:
	class CGameObject* mOwner;
	class CD2CharacterInfoComponent* mCharInfo;
	D2ItemInfo mInfo;
};

