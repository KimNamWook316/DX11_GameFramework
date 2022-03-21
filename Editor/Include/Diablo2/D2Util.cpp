#include "D2Util.h"
#include "../EditorInfo.h"
#include "State/PlayerIdleState.h"
#include "State/D2EnemyIdleState.h"
#include "State/D2AndarielIdleState.h"

eD2SpriteDir CD2Util::GetSpriteDir(const Vector2 dir)
{
	// South 축과의 각을 구한다.
	float dirAngle = Vector2(0.f, -1.f).Angle(dir);

	if (dirAngle <= 11.25f)
	{
		return eD2SpriteDir::S;
	}
	else if (dirAngle <= 33.75f)
	{
		if (dir.x > 0)
		{
			return eD2SpriteDir::SSE;
		}
		else
		{
			return eD2SpriteDir::SSW;
		}
	}
	else if (dirAngle <= 56.25f)
	{
		if (dir.x > 0)
		{
			return eD2SpriteDir::ES;
		}
		else
		{
			return eD2SpriteDir::SW;
		}
	}
	else if (dirAngle <= 78.75f)
	{
		if (dir.x > 0)
		{
			return eD2SpriteDir::EES;
		}
		else
		{
			return eD2SpriteDir::WWS;
		}
	}
	else if (dirAngle <= 101.25f)
	{
		if (dir.x > 0)
		{
			return eD2SpriteDir::E;
		}
		else
		{
			return eD2SpriteDir::W;
		}
	}
	else if (dirAngle <= 123.75f)
	{
		if (dir.x > 0)
		{
			return eD2SpriteDir::EEN;
		}
		else
		{
			return eD2SpriteDir::WWN;
		}
	}
	else if (dirAngle <= 146.25f)
	{
		if (dir.x > 0)
		{
			return eD2SpriteDir::NE;
		}
		else
		{
			return eD2SpriteDir::NW;
		}
	}
	else if (dirAngle <= 168.75f)
	{
		if (dir.x > 0)
		{
			return eD2SpriteDir::NNE;
		}
		else
		{
			return eD2SpriteDir::NNW;
		}
	}
	else if (dirAngle <= 191.25f)
	{
		return eD2SpriteDir::N;
	}
	else
	{
		assert(false);
	}
}

std::string CD2Util::SpriteDirToString(eD2SpriteDir dir)
{
	std::string out;
	switch (dir)
	{
	case eD2SpriteDir::S:
		out = "S";
		break;
	case eD2SpriteDir::SSW:
		out = "SSW";
		break;
	case eD2SpriteDir::SW:
		out = "SW";
		break;
	case eD2SpriteDir::WWS:
		out = "WWS";
		break;
	case eD2SpriteDir::W:
		out = "W";
		break;
	case eD2SpriteDir::WWN:
		out = "WWN";
		break;
	case eD2SpriteDir::NW:
		out = "NW";
		break;
	case eD2SpriteDir::NNW:
		out = "NNW";
		break;
	case eD2SpriteDir::N:
		out = "N";
		break;
	case eD2SpriteDir::NNE:
		out = "NNE";
		break;
	case eD2SpriteDir::NE:
		out = "NE";
		break;
	case eD2SpriteDir::EEN:
		out = "EEN";
		break;
	case eD2SpriteDir::E:
		out = "E";
		break;
	case eD2SpriteDir::EES:
		out = "EES";
		break;
	case eD2SpriteDir::ES:
		out = "ES";
		break;
	case eD2SpriteDir::SSE:
		out = "SSE";
		break;
	}
	return out;
}

std::string CD2Util::StateEnumToString(eD2InitialStateType type)
{
	std::string out;
	switch (type)
	{
	case eD2InitialStateType::D2PlayerIdle:
		out = "D2PlayerIdle";
		return out;
	case eD2InitialStateType::D2EnemyIdle:
		out = "D2EnemyIdle";
		return out;
	case eD2InitialStateType::D2AndarielIdle:
		out = "D2AndarielIdle";
		return out;
	}
	return out;
}

eD2InitialStateType CD2Util::StringToStateEnum(const std::string& typeString)
{
	if (typeString == "D2PlayerIdle")
	{
		return eD2InitialStateType::D2PlayerIdle;
	}
	else if (typeString == "D2EnemyIdle")
	{
		return eD2InitialStateType::D2EnemyIdle;
	}
	else if (typeString == "D2AndarielIdle")
	{
		return eD2InitialStateType::D2AndarielIdle;
	}
	else
	{
		return (eD2InitialStateType)(-1);
	}
}

std::string CD2Util::StateTypeToString(size_t type)
{
	std::string out;
	if (typeid(CPlayerIdleState).hash_code() == type)
	{
		out = "D2PlayerIdle";
		return out;
	}
	else if (typeid(CD2EnemyIdleState).hash_code() == type)
	{
		out = "D2EnemyIdle";
		return out;
	}
	return out;
}

size_t CD2Util::StateEnumToStateType(eD2InitialStateType type)
{
	switch (type)
	{
	case eD2InitialStateType::D2PlayerIdle:
		return typeid(CPlayerIdleState).hash_code();
	case eD2InitialStateType::D2EnemyIdle:
		return typeid(CD2EnemyIdleState).hash_code();
	case eD2InitialStateType::D2AndarielIdle:
		return typeid(CD2AndarielIdleState).hash_code();
	}
	return -1;
}

std::string CD2Util::D2SceneComponentTypeToString(eD2SceneComponentType type)
{
	std::string out;
	switch (type)
	{
	}
	return out;
}

std::string CD2Util::D2ObjectComponentTypeToString(eD2ObjectComponentType type)
{
	std::string out;
	switch (type)
	{
	case eD2ObjectComponentType::D2PlayerCollider:
		out = "D2PlayerCollider";
		return out;
	case eD2ObjectComponentType::D2EnemyCollider:
		out = "D2EnemyCollider";
		return out;
	case eD2ObjectComponentType::D2CharacterInfo:
		out = "D2CharacterInfo";
		return out;
	case eD2ObjectComponentType::D2State:
		out = "D2State";
		return out;
	case eD2ObjectComponentType::D2DataManager:
		out = "D2DataManager";
		return out;
	case eD2ObjectComponentType::D2NavAgent:
		out = "D2NavAgent";
		return out;
	case eD2ObjectComponentType::D2EnemyNavAgent:
		out = "D2EnemyNavAgent";
		break;
	case eD2ObjectComponentType::D2ObjectPool:
		out = "D2ObjectPool";
		return out;
	case eD2ObjectComponentType::D2Projectile:
		out = "D2Projectile";
		return out;
	case eD2ObjectComponentType::D2MeleeAttack:
		out = "D2MeleeAttack";
		return out;
	case eD2ObjectComponentType::D2PlayerSkill:
		out = "D2PlayerSkill";
		return out;
	case eD2ObjectComponentType::D2EnemySkill:
		out = "D2EnemySkill";
		return out;
	case eD2ObjectComponentType::D2Blaze:
		out = "D2Blaze";
		break;
	case eD2ObjectComponentType::D2FrozenOrb:
		out = "D2FrozenOrb";
		break;
	case eD2ObjectComponentType::D2Meteor:
		out = "D2Meteor";
		break;
	case eD2ObjectComponentType::D2MeteorFire:
		out = "D2MeteorFire";
		break;
	case eD2ObjectComponentType::D2MeteorTarget:
		out = "D2MeteorTarget";
		break;
	case eD2ObjectComponentType::D2Teleport:
		out = "D2Teleport";
		break;
	case eD2ObjectComponentType::D2EnemyMeleeAttack:
		out = "D2EnemyMeleeAttack";
		break;
	case eD2ObjectComponentType::D2Effect:
		out = "D2Effect";
		break;
	}
	return out;
}

eD2SceneComponentType CD2Util::StringToD2SceneComponentType(const std::string& typeString)
{
	return (eD2SceneComponentType)(-1);
}

eD2ObjectComponentType CD2Util::StringToD2ObjectComponentType(const std::string& typeString)
{
	if (typeString == "D2PlayerCollider")
	{
		return eD2ObjectComponentType::D2PlayerCollider;
	}
	else if (typeString == "D2EnemyCollider")
	{
		return eD2ObjectComponentType::D2EnemyCollider;
	}
	else if (typeString == "D2CharacterInfo")
	{
		return eD2ObjectComponentType::D2CharacterInfo;
	}
	else if (typeString == "D2State")
	{
		return eD2ObjectComponentType::D2State;
	}
	else if (typeString == "D2DataManager")
	{
		return eD2ObjectComponentType::D2DataManager;
	}
	else if (typeString == "D2NavAgent")
	{
		return eD2ObjectComponentType::D2NavAgent;
	}
	else if (typeString == "D2EnemyNavAgent")
	{
		return eD2ObjectComponentType::D2EnemyNavAgent;
	}
	else if (typeString == "D2ObjectPool")
	{
		return eD2ObjectComponentType::D2ObjectPool;
	}
	else if (typeString == "D2Projectile")
	{
		return eD2ObjectComponentType::D2Projectile;
	}
	else if (typeString == "D2MeleeAttack")
	{
		return eD2ObjectComponentType::D2MeleeAttack;
	}
	else if (typeString == "D2PlayerSkill")
	{
		return eD2ObjectComponentType::D2PlayerSkill;
	}
	else if (typeString == "D2EnemySkill")
	{
		return eD2ObjectComponentType::D2EnemySkill;
	}
	else if (typeString == "D2Blaze")
	{
		return eD2ObjectComponentType::D2Blaze;
	}
	else if (typeString == "D2FrozenOrb")
	{
		return eD2ObjectComponentType::D2FrozenOrb;
	}
	else if (typeString == "D2Meteor")
	{
		return eD2ObjectComponentType::D2Meteor;
	}
	else if (typeString == "D2MeteorFire")
	{
		return eD2ObjectComponentType::D2MeteorFire;
	}
	else if (typeString == "D2MeteorTarget")
	{
		return eD2ObjectComponentType::D2MeteorTarget;
	}
	else if (typeString == "D2Teleport")
	{
		return eD2ObjectComponentType::D2Teleport;
	}
	else if (typeString == "D2EnemyMeleeAttack")
	{
		return eD2ObjectComponentType::D2EnemyMeleeAttack;
	}
	else if (typeString == "D2Effect")
	{
		return eD2ObjectComponentType::D2Effect;
	}
	return (eD2ObjectComponentType)(-1);
}

std::string CD2Util::ElementTypeToString(eD2ElementType type)
{
	std::string out;
	switch (type)
	{
	case eD2ElementType::None:
		out = "None";
		break;
	case eD2ElementType::Fire:
		out = "Fire";
		break;
	case eD2ElementType::Ice:
		out = "Ice";
		break;
	case eD2ElementType::Lightning:
		out = "Lightning";
		break;
	case eD2ElementType::Poison:
		out = "Poison";
		break;
	}
	return out;
}

eD2ElementType CD2Util::StringToElementType(const std::string& typeString)
{
	if (typeString == "Fire")
	{
		return eD2ElementType::Fire;
	}
	else if (typeString == "None")
	{
		return eD2ElementType::None;
	}
	else if (typeString == "Ice")
	{
		return eD2ElementType::Ice;
	}
	else if (typeString == "Lightning")
	{
		return eD2ElementType::Lightning;
	}
	else if (typeString == "Poison")
	{
		return eD2ElementType::Poison;
	}
	return (eD2ElementType)(-1);
}

std::string CD2Util::AttackTypeToString(eD2AttackType type)
{
	std::string out;
	switch (type)
	{
	case eD2AttackType::Melee:
		out = "Melee";
		break;
	case eD2AttackType::Projectile:
		out = "Projectile";
		break;
	case eD2AttackType::Casting:
		out = "Casting";
		break;
	}
	return out;
}

eD2AttackType CD2Util::StringToAttackType(const std::string& typeString)
{
	if (typeString == "Melee")
	{
		return eD2AttackType::Melee;
	}
	else if (typeString == "Projectile")
	{
		return eD2AttackType::Projectile;
	}
	else if (typeString == "Casting")
	{
		return eD2AttackType::Casting;
	}
	return (eD2AttackType)(-1);
}

std::string CD2Util::SkilltreeNoToString(eD2SkillTreeNo no)
{
	std::string out;
	switch (no)
	{
	case eD2SkillTreeNo::Tree1:
		out = "Tree1";
		break;
	case eD2SkillTreeNo::Tree2:
		out = "Tree2";
		break;
	case eD2SkillTreeNo::Tree3:
		out = "Tree3";
		break;
	}
	return out;
}

eD2SkillTreeNo CD2Util::StringToSkilltreeNo(const std::string& noString)
{
	if ("Tree1" == noString)
	{
		return eD2SkillTreeNo::Tree1;
	}
	else if ("Tree2" == noString)
	{
		return eD2SkillTreeNo::Tree2;
	}
	else if ("Tree3" == noString)
	{
		return eD2SkillTreeNo::Tree3;
	}
	else
	{
		return (eD2SkillTreeNo)(-1);
	}
}

eD2ItemUseType CD2Util::StringToItemUseType(const std::string& str)
{
	if (str == "None")
	{
		return eD2ItemUseType::None;
	}
	else if (str == "Equip")
	{
		return eD2ItemUseType::Equip;
	}
	else if (str == "Usable")
	{
		return eD2ItemUseType::Usable;
	}
}

float CD2Util::CorrectSpeed(const Vector2& direction, const float speed)
{
	Vector2 axis = Vector2(1.0f, 0.f);

	float angle = direction.Angle(axis);

	if (angle >= 180.f)
	{
		angle -= 180.f;
	}

	float coeff = 0.f;

	if (angle <= 90.f)
	{
		angle = DegToRad(angle);
		coeff = ((-1 / PI) * angle) + 1.f;
	}
	else
	{
		angle = DegToRad(angle);
		coeff = angle / PI;
	}

	return speed * coeff;
}

