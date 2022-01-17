#pragma once

#include "Dia2Flags.h"

struct CharacterInfo
{
	Vector2 Dir;
	float CurMoveSpeed;
	float MaxMoveSpeed;

	CharacterInfo()	:
		Dir(0.f, -1.f),
		CurMoveSpeed(0.f),
		MaxMoveSpeed(0.f)
	{
	}
};

struct PlayerInfo
{
	CharacterInfo Info;
	ePlayerSpriteDir SpriteDir;

	PlayerInfo()	:
		Info{},
		SpriteDir(ePlayerSpriteDir::S)
	{
	}
};

struct ObjectInfo
{
	CharacterInfo Info;
	eObjectSpriteDir SpriteDir;

	ObjectInfo()	:
		Info{},
		SpriteDir(eObjectSpriteDir::S)
	{
	}
};
