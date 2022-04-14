#include "D2UsableItem.h"
#include "D2CharacterInfoComponent.h"

CD2UsableItem::CD2UsableItem()
{
}

CD2UsableItem::CD2UsableItem(const CD2UsableItem& item)	:
	CD2Item(item)
{
}

CD2UsableItem::~CD2UsableItem()
{
}

CD2UsableItem* CD2UsableItem::Clone()
{
	return new CD2UsableItem(*this);
}

void CD2UsableItem::UseItem()
{
	if (mInfo.Hp != 0)
	{
		mCharInfo->SetHp(mInfo.Hp);
	}
	
	if (mInfo.Mp != 0)
	{
		mCharInfo->SetMp(mInfo.Mp);
	}
}
