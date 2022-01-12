#pragma once

#include "Dia2Flags.h"

struct CharacterInfo
{
	Vector2 Dir;
	eCharDir SpriteDir;
	float CurMoveSpeed;
	float MaxMoveSpeed;

	CharacterInfo()	:
		Dir(0.f, -1.f),
		SpriteDir(eCharDir::S),
		CurMoveSpeed(0.f),
		MaxMoveSpeed(0.f)
	{
	}
};
