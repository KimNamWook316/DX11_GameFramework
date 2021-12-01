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

enum class eImageType
{
	Atlas,
	Frame,
	// 낱장 단위의 이미지 크기들이 모두 동일하다면,
	// 텍스쳐 내부에 밉맵이라는 것이 있어서
	// 밉맵 체인이라는 것을 이용해 arrayTexture라는 것을 만들어 쓸 수 있다.
	Array
};

enum class eSamplerType
{
	Point,
	Linear,
	Anisotropic,
};

enum class eRenderStateType
{
	Blend,
	Rasterizer,
	DepthStencil,
};