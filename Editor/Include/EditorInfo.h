#pragma once

#include "GameInfo.h"

enum class eCreateObjectType
{
	GameObject,
	Player,
};

// TODO : 엔진 기본 컴포넌트, 사용자 정의 컴포넌트 추가될 때마다 여기에 enum 추가해야 함
enum class eCreateSceneComponentType
{
	Sprite,
	StaticMesh,
	ColliderBox2D,
	ColliderCircle,
	ColliderPixel,
	Camera,
	Widget,
	Particle,
	Max
};
