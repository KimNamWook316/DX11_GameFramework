#pragma once

#include "D2Flag.h"
#include "GameInfo.h"

struct D2CharInfo
{
	std::string Name;
	float Speed;
	float MaxSpeed;
	Vector2 Dir;
	eD2SpriteDir eSpriteDir;
	float Hp;
	float MaxHp;
	float MaxCCTime;
	float Mp;
	float MaxMp;
	float MeleeBonus;
	float MagicBonus;
};

struct D2SkillInfo
{
	std::string Name;
	float Damage;
	float Speed;
	Vector2 Dir;
	eD2ElementType eElementType;
	float LifeTime;
};

struct D2ItemInfo
{
	std::string Name;
	int UniqueID;
	eD2ItemType eType;
	int CellX;
	int CellY;
	eD2ItemUseType eUseType;
	int Hp;
	int Mp;
	int Melee;
	int Magic;
};

struct D2ItemSlotInfo
{
	class CD2Item* Item;
	int SlotRootIndex;
};
