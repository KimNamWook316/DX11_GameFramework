#include "Util.h"

std::string CUtil::CollisionChannelToString(eCollisionChannel eChan)
{
	std::string out;

	switch (eChan)
	{
	case eCollisionChannel::Object:
		out = "Object";
		break;
	case eCollisionChannel::Player:
		out = "Player";
		break;
	case eCollisionChannel::Monster:
		out = "Monster";
		break;
	case eCollisionChannel::PlayerAttack:
		out = "PlayerAttack";
		break;
	case eCollisionChannel::MonsterAttack:
		out = "MonsterAttack";
		break;
	case eCollisionChannel::Custom1:
		out = "Custom1";
		break;
	case eCollisionChannel::Custom2:
		out = "Custom2";
		break;
	case eCollisionChannel::Custom3:
		out = "Custom3";
		break;
	case eCollisionChannel::Custom4:
		out = "Custom4";
		break;
	case eCollisionChannel::Custom5:
		out = "Custom5";
		break;
	case eCollisionChannel::Custom6:
		out = "Custom6";
		break;
	case eCollisionChannel::Custom7:
		out = "Custom7";
		break;
	case eCollisionChannel::Custom8:
		out = "Custom8";
		break;
	case eCollisionChannel::Custom9:
		out = "Custom9";
		break;
	case eCollisionChannel::Custom10:
		out = "Custom10";
		break;
	default:
		assert(false);
		break;
	}

	return out;
}

std::string CUtil::CollsionInteractionToString(eCollisionInteraction eInterAction)
{
	return eCollisionInteraction::Collision == eInterAction ? "TRUE" : "FALSE";
}

std::string CUtil::TileShapeToString(eTileShape eTileShape)
{
	std::string out;
	switch (eTileShape)
	{
	case eTileShape::Rect:
		out = "Rect";
		break;
	case eTileShape::Rhombus:
		out = "Rhombus";
		break;
	default:
		assert(false);
		break;
	}
	return out;
}

eTileShape CUtil::StringToTileShape(const std::string& str)
{
	if (str == "Rect")
	{
		return eTileShape::Rect;
	}
	else if (str == "Rhombus")
	{
		return eTileShape::Rhombus;
	}

	assert(false);
	return (eTileShape)-1;
}

std::string CUtil::TileTypeToString(eTileType eTileType)
{
	std::string out;
	switch (eTileType)
	{
	case eTileType::Normal:
		out = "Normal";
		break;
	case eTileType::Wall:
		out = "Wall";
		break;
	default:
		assert(false);
		break;
	}
	return out;
}

eTileType CUtil::StringToTileType(const std::string& str)
{
	if (str == "Normal")
	{
		return eTileType::Normal;
	}
	else if (str == "Wall")
	{
		return eTileType::Wall;
	}
	
	assert(false);
	return (eTileType)-1;
}

std::string CUtil::SceneComponentTypeToString(eSceneComponentType eType)
{
	std::string out;
	switch (eType)
	{
	case eSceneComponentType::Sprite:
		out = "Sprite";
		break;
	case eSceneComponentType::StaticMesh:
		out = "StaticMesh";
		break;
	case eSceneComponentType::ColliderBox2D:
		out = "ColliderBox2D";
		break;
	case eSceneComponentType::ColliderCircle:
		out = "ColliderCircle";
		break;
	case eSceneComponentType::ColliderPixel:
		out = "ColliderPixel";
		break;
	case eSceneComponentType::Camera:
		out = "Camera";
		break;
	case eSceneComponentType::Widget:
		out = "Widget";
		break;
	case eSceneComponentType::Particle:
		out = "Particle";
		break;
	case eSceneComponentType::TileMap:
		out = "TileMap";
		break;
	default:
		assert(false);
		break;
	}
	return out;
}
