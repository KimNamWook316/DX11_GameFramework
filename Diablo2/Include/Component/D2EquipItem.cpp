#include "D2EquipItem.h"
#include "D2CharacterInfoComponent.h"

CD2EquipItem::CD2EquipItem()
{
}

CD2EquipItem::CD2EquipItem(const CD2EquipItem& item)	:
	CD2Item(item)
{
}

CD2EquipItem::~CD2EquipItem()
{
}

CD2EquipItem* CD2EquipItem::Clone()
{
	return new CD2EquipItem(*this);
}

void CD2EquipItem::Equip()
{
	if (mInfo.Hp != 0)
	{
		mCharInfo->SetMaxHp(mInfo.Hp);
	}
	if (mInfo.Mp != 0)
	{
		mCharInfo->SetMaxMp(mInfo.Mp);
	}
	if (mInfo.Melee != 0)
	{
		mCharInfo->SetMeleeBonus(mInfo.Melee);
	}
	if (mInfo.Magic != 0)
	{
		mCharInfo->SetMeleeBonus(mInfo.Magic);
	}
}

void CD2EquipItem::UnEquip()
{
	if (mInfo.Hp != 0)
	{
		mCharInfo->SetMaxHp(-mInfo.Hp);
	}
	if (mInfo.Mp != 0)
	{
		mCharInfo->SetMaxMp(-mInfo.Mp);
	}
	if (mInfo.Melee != 0)
	{
		mCharInfo->SetMeleeBonus(-mInfo.Melee);
	}
	if (mInfo.Magic != 0)
	{
		mCharInfo->SetMeleeBonus(-mInfo.Magic);
	}
}
