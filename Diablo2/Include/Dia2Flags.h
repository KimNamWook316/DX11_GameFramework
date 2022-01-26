#pragma once

enum class eSpriteDir
{
	S,
	SSW,
	SW,
	WWS,
	W,
	WWN,
	NW,
	NNW,
	N,
	NNE,
	NE,
	EEN,
	E,
	EES,
	ES,
	SSE,
};

enum class eMouseClickState
{
	MouseLButton,
	MouseRButton,
};

enum class ePlayerState
{
	TownIdle,
	TownWalk,
	Idle,
	Run,
	Attack1,
	Attack2,
	Hit,
	Block,
	Kick,
	Special1,
	Special2,
	End
};
