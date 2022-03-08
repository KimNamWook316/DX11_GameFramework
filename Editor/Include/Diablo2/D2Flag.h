#pragma once

enum class eD2SpriteDir
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

enum class eD2StateType
{
	D2PlayerIdle,
	D2PlayerRun,
	MAX
};

enum class eD2SceneComponentType
{
	Max,
};

enum class eD2ObjectComponentType
{
	D2CharacterInfo,
	D2DataManager,
	D2Projectile,
	Max,
};

enum class eD2ElementType
{
	Fire,
	Ice,
	Lightning,
	Max
};

enum class eD2CharInfoEventType
{
    HpDec,
    HpInc,
    SpeedUp,
    SpeedDown,
    ChangeDir,
    Max
};

enum class eD2AttackType
{
	Melee,
	Projectile,
	Max
};
