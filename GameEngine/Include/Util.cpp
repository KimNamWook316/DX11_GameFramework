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
	case eTileType::CornerN:
		out = "CornerN";
		break;
	case eTileType::CornerE:
		out = "CornerE";
		break;
	case eTileType::CornerS:
		out = "CornerS";
		break;
	case eTileType::CornerW:
		out = "CornerW";
		break;
	case eTileType::InnerCornerN:
		out = "InnerConnerN";
		break;
	case eTileType::InnerCornerE:
		out = "InnerConnerE";
		break;
	case eTileType::InnerCornerS:
		out = "InnerConnerS";
		break;
	case eTileType::InnerCornerW:
		out = "InnerConnerW";
		break;
	case eTileType::WallNE:
		out = "WallNE";
		break;
	case eTileType::WallSE:
		out = "WallSE";
		break;
	case eTileType::WallSW:
		out = "WallSW";
		break;
	case eTileType::WallNW:
		out = "WallNW";
		break;
	case eTileType::WallEndNETop:
		out = "WallEndNETop";
		break;
	case eTileType::WallEndNEBottom:
		out = "WallEndNEBottom";
		break;
	case eTileType::WallEndSETop:
		out = "WallEndSETop";
		break;
	case eTileType::WallEndSEBottom:
		out = "WallEndSEBottom";
		break;
	case eTileType::WallEndSWTop:
		out = "WallEndSWTop";
		break;
	case eTileType::WallEndSWBottom:
		out = "WallEndSWBottom";
		break;
	case eTileType::WallEndNWTop:
		out = "WallEndNWTop";
		break;
	case eTileType::WallEndNWBottom:
		out = "WallEndNWBottom";
		break;
	case eTileType::EntryNELeft:
		out = "EntryNELeft";
		break;
	case eTileType::EntryNERight:
		out = "EntryNERight";
		break;
	case eTileType::EntrySELeft:
		out = "EntrySELeft";
		break;
	case eTileType::EntrySERight:
		out = "EntrySERight";
		break;
	case eTileType::EntrySWLeft:
		out = "EntrySWLeft";
		break;
	case eTileType::EntrySWRight:
		out = "EntrySWRight";
		break;
	case eTileType::EntryNWLeft:
		out = "EntryNWLeft";
		break;
	case eTileType::EntryNWRight:
		out = "EntryNWRight";
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
	else if (str == "CornerN")
	{
		return eTileType::CornerN;
	}
	else if (str == "CornerE")
	{
		return eTileType::CornerE;
	}
	else if (str == "CornerS")
	{
		return eTileType::CornerS;
	}
	else if (str == "CornerW")
	{
		return eTileType::CornerW;
	}
	else if (str == "InnerConnerN")
	{
		return eTileType::InnerCornerN;
	}
	else if (str == "InnerConnerE")
	{
		return eTileType::InnerCornerE;
	}
	else if (str == "InnerConnerS")
	{
		return eTileType::InnerCornerS;
	}
	else if (str == "InnerConnerW")
	{
		return eTileType::InnerCornerW;
	}
	else if (str == "WallNE")
	{
		return eTileType::WallNE;
	}
	else if (str == "WallSE")
	{
		return eTileType::WallSE;
	}
	else if (str == "WallSW")
	{
		return eTileType::WallSW;
	}
	else if (str == "WallNW")
	{
		return eTileType::WallNW;
	}
	else if (str == "WallEndNETop")
	{
		return eTileType::WallEndNETop;
	}
	else if (str == "WallEndNEBottom")
	{
		return eTileType::WallEndNEBottom;
	}
	else if (str == "WallEndSETop")
	{
		return eTileType::WallEndSETop;
	}
	else if (str == "WallEndSEBottom")
	{
		return eTileType::WallEndSEBottom;
	}
	else if (str == "WallEndSWTop")
	{
		return eTileType::WallEndSWTop;
	}
	else if (str == "WallEndSWBottom")
	{
		return eTileType::WallEndSWBottom;
	}
	else if (str == "WallEndNWTop")
	{
		return eTileType::WallEndNWTop;
	}
	else if (str == "WallEndNWBottom")
	{
		return eTileType::WallEndNWBottom;
	}
	else if (str == "EntryNELeft")
	{
		return eTileType::EntryNELeft;
	}
	else if (str == "EntryNERight")
	{
		return eTileType::EntryNERight;
	}
	else if (str == "EntrySELeft")
	{
		return eTileType::EntrySELeft;
	}
	else if (str == "EntrySERight")
	{
		return eTileType::EntrySERight;
	}
	else if (str == "EntrySWLeft")
	{
		return eTileType::EntrySWLeft;
	}
	else if (str == "EntrySWRight")
	{
		return eTileType::EntrySWRight;
	}
	else if (str == "EntryNWLeft")
	{
		return eTileType::EntryNWLeft;
	}
	else if (str == "EntryNWRight")
	{
		return eTileType::EntryNWRight;
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
	case eSceneComponentType::ProcedualMapGenerator:
		out = "ProcedualMapGenerator";
		break;
	default:
		assert(false);
		break;
	}
	return out;
}

std::string CUtil::ObjectComponentTypeToString(eObjectComponentType eType)
{
	std::string out;
	switch (eType)
	{
	case eObjectComponentType::Dissolve:
		out = "Dissolve";
		break;
	case eObjectComponentType::NavAgent:
		out = "NavAgent";
		break;
	default:
		assert(false);
		break;
	}
	return out;
}
