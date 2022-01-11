#pragma once

#include "Dia2Flags.h"

struct CharacterInfo
{
	Vector2 Dir;
	eCharDir SpriteDir;
	float MoveSpeed;

	CharacterInfo()	:
		SpriteDir(eCharDir::S),
		MoveSpeed(100.f)
	{
	}
};
