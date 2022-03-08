#pragma once

#include "GameInfo.h"

class CUtil
{
public:
	static std::string CollisionChannelToString(eCollisionChannel eChan);

	static std::string CollsionInteractionToString(eCollisionInteraction eInterAction);

	static std::string TileShapeToString(eTileShape eTileShape);
	static eTileShape  StringToTileShape(const std::string& str);

	static std::string TileTypeToString(eTileType eTileType);
	static eTileType StringToTileType(const std::string& str);

	static std::string SceneComponentTypeToString(eSceneComponentType eType);
	static std::string ObjectComponentTypeToString(eObjectComponentType eType);

	static std::string CameraTypeToString(eCameraType eType);
	static eCameraType StringToCameraType(const std::string& str);
};

