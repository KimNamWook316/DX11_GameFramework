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
};

struct D2SkillInfo
{
	std::string Name;
	float Damage;
	float Speed;
	Vector2 Dir;
	eD2ElementType eElementType;
};
