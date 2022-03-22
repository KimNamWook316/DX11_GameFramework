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
	MAX,
};

enum class eSceneComponentType
{
	Scene,
	Sprite,
	StaticMesh,
	ColliderBox2D,
	ColliderCircle,
	ColliderPixel,
	Camera,
	Widget,
	Particle,
	TileMap,
	ProcedualMapGenerator,
	Max
};

enum class eObjectComponentType
{
	Dissolve,
	NavAgent,
	State,
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
	Max
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
	CornerN,
	CornerE,
	CornerS,
	CornerW,
	InnerCornerN,
	InnerCornerE,
	InnerCornerS,
	InnerCornerW,
	WallNE,
	WallSE,
	WallSW,
	WallNW,
	WallEndNETop,
	WallEndNEBottom,
	WallEndSETop,
	WallEndSEBottom,
	WallEndSWTop,
	WallEndSWBottom,
	WallEndNWTop,
	WallEndNWBottom,
	EntryNELeft,
	EntryNERight,
	EntrySELeft,
	EntrySERight,
	EntrySWLeft,
	EntrySWRight,
	EntryNWLeft,
	EntryNWRight,
	Reserved, // 몬스터 AI 목적지 지정을 위한 타일 타입
	Max,
};

enum class eNavNodeType
{
	None,
	Open,
	Closed,
};

enum class eProcedualObjectType
{
	Player,
	Enemy,
	Max
};

enum class eCellState
{
	Normal,
	Hovered,
	Clicked,
	Max
};
