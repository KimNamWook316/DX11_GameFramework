#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <string.h>
#include <assert.h>

#include "Flag.h"

using namespace DirectX;

#define PI 3.14159f

static float DegToRad(float angle)
{
	return angle / 180.f * PI;
}

static float RadToDeg(float angle)
{
	return angle * 180.f / PI;
}