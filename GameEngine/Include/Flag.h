#pragma once

enum eAXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

enum class eBufferType
{
	VERTEX,
	INDEX,
};

enum class eShaderType
{
	GRAPHIC,
	COMPUTE,
};

enum class eComponentType
{
	SCENE_COMP,
	OBJ_COMP,
};

enum class eConstantBufferShaderTypeFlags
{
	Vertex = 0x1,
	Pixel = 0x2,
	Domain = 0x4,
	Hull = 0x8,
	Geometry = 0x10,
	Compute = 0x20,
	Graphic = Vertex | Pixel | Domain | Hull | Geometry,
	All = Vertex | Pixel | Domain | Hull | Geometry | Compute
};

enum class eInputType
{
	Direct,
	Window
};

enum eKeyState
{
	KeyState_Down,
	KeyState_Push,
	KeyState_Up,
	KeyState_Max
};