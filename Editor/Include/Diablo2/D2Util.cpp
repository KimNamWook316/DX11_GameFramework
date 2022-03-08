#include "D2Util.h"
#include "../EditorInfo.h"
#include "State/PlayerIdleState.h"
#include "State/PlayerRunState.h"

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

std::string CD2Util::StateEnumToString(eD2StateType type)
{
	std::string out;
	switch (type)
	{
	case eD2StateType::D2PlayerIdle:
		out = "D2PlayerIdle";
		return out;
	case eD2StateType::D2PlayerRun:
		out = "D2PlayerRun";
		return out;
	}
	return out;
}

eD2StateType CD2Util::StringToStateEnum(const std::string& typeString)
{
	if (typeString == "D2PlayerIdle")
	{
		return eD2StateType::D2PlayerIdle;
	}
	else if (typeString == "D2PlayerRun")
	{
		return eD2StateType::D2PlayerRun;
	}
	else
	{
		return (eD2StateType)(-1);
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
	else if (typeid(CPlayerRunState).hash_code() == type)
	{
		out = "D2PlayerRun";
		return out;
	}
	return out;
}

size_t CD2Util::StateEnumToStateType(eD2StateType type)
{
	switch (type)
	{
	case eD2StateType::D2PlayerIdle:
		return typeid(CPlayerIdleState).hash_code();
	case eD2StateType::D2PlayerRun:
		return typeid(CPlayerRunState).hash_code();
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
	case eD2ObjectComponentType::D2CharacterInfo:
		out = "D2CharacterInfo";
		return out;
	case eD2ObjectComponentType::D2DataManager:
		out = "D2DataManager";
		return out;
	case eD2ObjectComponentType::D2Projectile:
		out = "D2Projectile";
		return out;
	}
	return out;
}

eD2SceneComponentType CD2Util::StringToD2SceneComponentType(const std::string& typeString)
{
	return (eD2SceneComponentType)(-1);
}

eD2ObjectComponentType CD2Util::StringToD2ObjectComponentType(const std::string& typeString)
{
	if (typeString == "D2CharacterInfo")
	{
		return eD2ObjectComponentType::D2CharacterInfo;
	}
	else if (typeString == "D2DataManager")
	{
		return eD2ObjectComponentType::D2DataManager;
	}
	else if (typeString == "D2Projectile")
	{
		return eD2ObjectComponentType::D2Projectile;
	}
	return (eD2ObjectComponentType)(-1);
}

std::string CD2Util::ElementTypeToString(eD2ElementType type)
{
	std::string out;
	switch (type)
	{
	case eD2ElementType::Fire:
		out = "Fire";
		break;
	case eD2ElementType::Ice:
		out = "Ice";
		break;
	case eD2ElementType::Lightning:
		out = "Lightning";
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
	else if (typeString == "Ice")
	{
		return eD2ElementType::Ice;
	}
	else if (typeString == "Lightning")
	{
		return eD2ElementType::Lightning;
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
	return (eD2AttackType)(-1);
}

