#pragma once

#include "GameInfo.h"

enum class eCreateObjectType
{
	GameObject,
	Player,
};

// TODO : ���� �⺻ ������Ʈ, ����� ���� ������Ʈ �߰��� ������ ���⿡ enum �߰��ؾ� ��
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
