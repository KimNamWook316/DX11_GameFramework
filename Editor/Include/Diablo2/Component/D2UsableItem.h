#pragma once
#include "D2Item.h"
class CD2UsableItem :
    public CD2Item
{
	friend class CD2ItemTable;

protected:
	CD2UsableItem();
	virtual ~CD2UsableItem();

public:
	void UseItem();
};

