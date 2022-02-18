#pragma once

enum class eEngineSpace
{
	Space2D,	
	Space3D,
};

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

enum class eBufferShaderTypeFlags
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
	// ���� ������ �̹��� ũ����� ��� �����ϴٸ�,
	// �ؽ��� ���ο� �Ӹ��̶�� ���� �־
	// �Ӹ� ü���̶�� ���� �̿��� arrayTexture��� ���� ����� �� �� �ִ�.
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
	MAX,
};

enum class eSceneComponentType
{
	Sprite,
	StaticMesh,
	ColliderBox2D,
	ColliderCircle,
	ColliderPixel,
	Camera,
	Widget,
	Particle,
	TileMap,
	Max
};

enum class eCollisionChannel
{
	Object,
	Player,
	Monster,
	PlayerAttack,
	MonsterAttack,
	Custom1,
	Custom2,
	Custom3,
	Custom4,
	Custom5,
	Custom6,
	Custom7,
	Custom8,
	Custom9,
	Custom10,
	Max
};

enum class eCollisionInteraction
{
	Ignore,
	Collision,
};

enum class eCollisionState
{
	Enter,
	Exit,
	Max
};

enum class eColliderType
{
	Box2D,
	Circle2D,
	Pixel,
};

enum class eCameraType
{
	Camera2D,
	Camera3D,
	UI,
};

enum class ePixelCollisionType
{
	ColorIgnore,
	ColorConfirm,
	AlphaIgnore,
	AlphaConfirm,
};

enum class eButtonState
{
	Normal,
	Hovered,
	Clicked,
	Disabled,
	Max,
};

enum class eButtonSoundState
{
	Hovered,
	Clicked,
	Max,
};

enum class eWidgetComponentSpace
{
	Screen,
	World,
};

enum class eProgressBarDir
{
	RightToLeft,
	LeftToRight,
	TopToBottom,
	BottomToTop,
};

enum class eSliderBarDir
{
	Vertical,
	Horizontal,
};

enum class eMouseState
{
	Normal,
	State1,
	State2,
	State3,
	State4,
	State5,
	State6,
	State7,
	State8,
	State9,
	State10,
	Max
};

enum class eTileShape
{
	Rect,
	Rhombus,
};

enum class eTileType
{
	Normal,
	Wall,
	Max
};

enum class eNavNodeType
{
	None,
	Open,
	Closed,
};
