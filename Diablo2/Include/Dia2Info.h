#pragma once

#include "GameInfo.h"
#include "Dia2Flags.h"

struct CharacterInfo
{
	eD2SpriteDir SpriteDir;
	Vector2 Dir;
	float CurMoveSpeed;
	float MaxMoveSpeed;

	CharacterInfo()	:
		SpriteDir(eD2SpriteDir::S),
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
