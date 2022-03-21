#pragma once
#include "D2Item.h"
class CD2EquipItem :
    public CD2Item
{
	friend class CD2ItemTable;

protected:
	CD2EquipItem();
	virtual ~CD2EquipItem();

public:
	void Equip();
	void UnEquip();
};

