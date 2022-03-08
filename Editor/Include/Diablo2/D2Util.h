#pragma once

#include "GameInfo.h"
#include "D2Info.h"
#include "../EditorInfo.h"

class CD2Util
{
public:
	static eD2SpriteDir GetSpriteDir(const Vector2 dir);
	static std::string SpriteDirToString(eD2SpriteDir dir);

	// State
	static std::string StateEnumToString(eD2StateType type);
	static eD2StateType StringToStateEnum(const std::string& typeString);
	static std::string StateTypeToString(size_t type);
	static size_t StateEnumToStateType(eD2StateType type);

	// Component
	static std::string D2SceneComponentTypeToString(eD2SceneComponentType type);
	static std::string D2ObjectComponentTypeToString(eD2ObjectComponentType type);
	static eD2SceneComponentType StringToD2SceneComponentType(const std::string& typeString);
	static eD2ObjectComponentType StringToD2ObjectComponentType(const std::string& typeString);

	// Element
	static std::string ElementTypeToString(eD2ElementType type);
	static eD2ElementType StringToElementType(const std::string& typeString);

	// SkillType
	static std::string AttackTypeToString(eD2AttackType type);
	static eD2AttackType StringToAttackType(const std::string& typeString);
};

