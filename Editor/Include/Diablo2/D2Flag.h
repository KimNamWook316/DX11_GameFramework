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
	D2State,
	D2DataManager,
	D2PlayerSkill,
	D2Projectile,
	D2MeleeAttack,
	D2Blaze,
	D2FrozenOrb,
	D2Meteor,
	D2MeteorFire,
	D2MeteorTarget,
	D2Teleport,
	Max,
};

enum class eD2ElementType
{
	None,
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
	CC,
    Max
};

enum class eD2AttackType
{
	Melee,
	Projectile,
	Casting,
	Max
};

enum class eD2SkillTreeNo
{
    Tree1,
    Tree2,
    Tree3,
    Max
};

