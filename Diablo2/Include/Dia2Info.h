#pragma once

#include "Dia2Flags.h"

struct CharacterInfo
{
	eSpriteDir SpriteDir;
	Vector2 Dir;
	float CurMoveSpeed;
	float MaxMoveSpeed;

	CharacterInfo()	:
		SpriteDir(eSpriteDir::S),
		Dir(0.f, -1.f),
		CurMoveSpeed(0.f),
		MaxMoveSpeed(0.f)
	{
	}
};

struct MouseInputInfo
{
    eMouseClickState State;
    Vector2 Position;
};
