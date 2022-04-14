#pragma once
#include "D2Item.h"
class CD2EquipItem :
    public CD2Item
{
	friend class CD2ItemTable;

protected:
	CD2EquipItem();
	CD2EquipItem(const CD2EquipItem& item);
	virtual ~CD2EquipItem();

public:
	virtual CD2EquipItem* Clone() override;

public:
	void Equip();
	void UnEquip();
};

