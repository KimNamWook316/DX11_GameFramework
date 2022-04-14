#pragma once
#include "D2Item.h"
class CD2UsableItem :
    public CD2Item
{
	friend class CD2ItemTable;

protected:
	CD2UsableItem();
	CD2UsableItem(const CD2UsableItem& item);
	virtual ~CD2UsableItem();

public:
	virtual CD2UsableItem* Clone() override;

public:
	void UseItem();
};

