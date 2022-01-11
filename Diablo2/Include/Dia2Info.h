#pragma once

#include "Dia2Flags.h"

struct CharacterInfo
{
	eCharDir Direction;
	float MoveSpeed;

	CharacterInfo()	:
		Direction(eCharDir::S),
		MoveSpeed(100.f)
	{
	}
};
