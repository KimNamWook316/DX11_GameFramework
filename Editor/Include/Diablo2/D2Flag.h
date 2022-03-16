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

enum class eD2InitialStateType
{
	D2PlayerIdle,
	D2EnemyIdle,
	MAX
};

enum class eD2SceneComponentType
{
	Max,
};

enum class eD2ObjectComponentType
{
	D2PlayerCollider,
	D2EnemyCollider,
	D2CharacterInfo,
	D2State,
	D2DataManager,
	D2NavAgent,
	D2EnemyNavAgent,
	D2ObjectPool,
	D2PlayerSkill,
	D2EnemySkill,
	D2Projectile,
	D2MeleeAttack,
	D2Blaze,
	D2FrozenOrb,
	D2Meteor,
	D2MeteorFire,
	D2MeteorTarget,
	D2Teleport,
	D2EnemyMeleeAttack,
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
	MpDec,
	MpInc,
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

