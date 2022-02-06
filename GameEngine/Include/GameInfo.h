#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <Windows.h>
#include <stdio.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <crtdbg.h>
#include <functional>
#include <string>
#include <stack>
#include <queue>
#include <process.h>
#include <math.h>
#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include <d2d1.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "Resource/Texture/DirectXTex.h"
#include "fmod.hpp"

#ifdef _DEBUG

#pragma comment(lib, "DirectXTex_Debug.lib")

#else

#pragma comment(lib, "DirectXTex.lib")

#endif // _DEBUG

#pragma comment(lib, "fmod64_vc.lib")

#define ROOT_PATH		"Root"
#define SHADER_PATH		"Shader"
#define TEXTURE_PATH	"Texture"
#define FONT_PATH		"Font" 
#define ANIMATION_PATH	"Animation"
#define SCENE_PATH		"Scene"
#define SOUND_PATH		"Sound"

#define SAFE_DELETE(p)  if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }
#define SAFE_RELEASE(p) if(p) { p->Release(); p = nullptr; }

#define DECLARE_SINGLE(Type)\
private:\
	static Type* mInst;\
public:\
	static Type* GetInst(){\
		if(!mInst)\
			mInst = new Type;\
		return mInst;\
	}\
static void DestroyInst() { \
		SAFE_DELETE(mInst);\
	}\
private:\
	Type();\
	~Type();

#define DEFINITION_SINGLE(Type) Type* Type::mInst = nullptr;

struct Resolution 
{
	unsigned int Width;
	unsigned int Height;
};

// 위치 정보와 색 정보만을 담은 Vertex. 다른 정보를 담은 Vertex가 무수히 많이 존재할 수 있다.
// 정점 연결이 시계방향일 경우 앞면으로 취급한다. 나중에 Back-Face Culling하여, 보이지 않는 뒷면 렌더링하지 않는 기술에서 사용한다.
struct VertexColor
{
	Vector3 Pos;
	Vector4 Color;

	VertexColor()
	{
	}

	VertexColor(const Vector3& _pos, const Vector4& _color)
		: Pos(_pos)
		, Color(_color)
	{
	}
};

// 텍스쳐 매핑할 UV좌표를 가지고 있는 Vertex
struct VertexUV
{
	Vector3 Pos;
	Vector2 UV;

	VertexUV()
	{
	}

	VertexUV(const Vector3& pos, const Vector2& uv) :
		Pos(pos),
		UV(uv)
	{
	}
};

// 정점 정보를 담은 버퍼
struct VertexBuffer
{
	ID3D11Buffer*	Buffer;
	int				Size;	// Vertex하나의 크기
	int				Count;	// Vertex의 총 개수
	
	VertexBuffer()
		: Buffer(nullptr)
		, Size(0)
		, Count(0)
	{
	}

	~VertexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};

// 정점을 연결하는 Index를 담은 버퍼
struct IndexBuffer
{
	ID3D11Buffer*	Buffer;
	int				Size;	// Index 하나의 크기
	int				Count;	// Index 개수
	DXGI_FORMAT		Fmt;	// 출력할 때 필요한 Format을 담고 있다.

	IndexBuffer()
		: Buffer(nullptr)
		, Size(0)
		, Count(0)
		, Fmt(DXGI_FORMAT_UNKNOWN)
	{
	}

	~IndexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};

// 메쉬 정보를 담고 있는 컨테이너
struct MeshContainer
{
	// 버텍스 버퍼
	VertexBuffer	VB;

	// 메쉬가 Subset으로 나뉘어 있는 경우, 인덱스를 쪼개서 관리한다.
	// Subset으로 나뉘는 이유는, 3D리소스의 경우 Material(재질)이 다르게 설정된 경우가 다분하기 떄문이다.
	// 하지만 VertexBuffer는 그냥 정점 정보일 뿐이고, 실제로 이어주기 위해서 Index가 필요하기 때문에 인덱스 버퍼만 쪼개서 관리한다.
	std::vector<IndexBuffer>	vecIB;

	// 정점을 이어주는 방식이다.
	D3D11_PRIMITIVE_TOPOLOGY	Primitive;	
};

// Shader cbuffer에 대응하는 버퍼 내용을 담고 있는 구조체
struct TransformCBuffer
{
	Matrix MatWorld;
	Matrix MatView;
	Matrix MatProj;
	Matrix MatWV;
	Matrix MatWVP;
	Matrix MatVP;
	Vector3 Pivot;
	float Dummy1;
	Vector3 MeshSize;
	// 16byte 정렬을 위한 Dummy Data.
	float Dummy2;
};

struct MaterialCBuffer
{
	Vector4 BaseColor;
	float	Opacity;
	Vector3 Dummy;
};

struct AnimationFrameData
{
	Vector2 Start;
	Vector2 Size;
};

struct Animation2DCBuffer
{
	Vector2 Animation2DStartUV;
	Vector2 Animation2DEndUV;
	int Animation2DType;
	Vector3 Dummy;
};

struct Standard2DCBuffer
{
	int AnimationEnable;
	Vector3 Dummy;
};

struct FindComponentName
{
	std::string Name;
	std::string ParentName;
};

struct CollisionProfile
{
	std::string Name;
	eCollisionChannel Channel; // 이 채널이 어떤 채널에 속해있는지
	bool bCollisionEnable;

	std::vector<eCollisionInteraction> vecInteraction; // 각 채널에 대해 충돌하는지 아닌지 ( 총 채널수만큼 사이즈)
};

struct CollisionResult
{
	class CColliderComponent* Src;
	class CColliderComponent* Dest;
	Vector3 HitPoint;

	CollisionResult()	:
		Src(nullptr),
		Dest(nullptr)
	{
	}
};

// for collider
struct Box2DInfo
{
	Vector2 Center;
	Vector2 Axis[2];
	Vector2 Length; // Collider Box의 너비, 높이의 절반
	Vector2 Min;	// 가장 낮은 좌표값
	Vector3 Max;	// 가장 높은 좌표값
};

struct CircleInfo
{
	Vector2 Center;
	float Radius;
	Vector2 Min;
	Vector2 Max;
};

struct PixelInfo
{
	unsigned char* Pixel; // 이미지 배열형태로 저장
	unsigned int Width;
	unsigned int Height;
	ePixelCollisionType Type;
	Vector2 Center;
	Vector2 Min;
	Vector2 Max;
	Box2DInfo Box;
	unsigned char Color[4]; // 4byte r, g, b, a 배열
	ID3D11ShaderResourceView* SRV;
	int RefCount; // 동일한 텍스처를 여러 개 생성할 필요가 없기 때문에, Ref Check해서 공유

	PixelInfo()	:
		RefCount(1),
		Pixel(nullptr),
		SRV(nullptr),
		Width(0),
		Height(0),
		Box{},
		Color{},
		Type(ePixelCollisionType::ColorConfirm)
	{
	}
};

struct ColliderCBuffer
{
	Vector4 Color;
	Matrix MatWVP;
};

struct WidgetCBuffer
{
	Vector4 Tint;
	Matrix MatWP;
	int UseTexture;
	float Opacity;
	Vector2 Dummy;
};

struct ProgressBarCBuffer
{
	float Percent;
	int Dir;
	Vector2 Empty;
};

struct ParticleCBuffer
{
	unsigned int SpawnEnable; // 현재 파티클 생성 여부
	Vector3 StartMin;
	Vector3 StartMax;
	unsigned int SpawnCountMax;
	Vector3 ScaleMin;
	float LifeTimeMin;
	Vector3 ScaleMax;
	float LifeTimeMax;
	Vector4 ColorMin;
	Vector4 ColorMax;
	float SpeedMin;
	float SpeedMax;
	int IsMove;
	int IsGravity;
	Vector3 MoveDir;
	int Is2D;
	Vector3 MoveAngle;
	float Dummy;
};

struct GlobalCBuffer
{
	float DeltaTime;
	float AccTime; // 누적 시간
	Vector2 Resolution;
	Vector2 NoiseResolution; // 랜덤 함수를 위한 노이즈 텍스쳐 해상도
	Vector2 Dummy;
};

struct ParticleInfo
{
	Vector3 WorldPos;
	Vector3 Dir;
	float Speed;
	float LifeTime;
	float LifeTimeMax;
	float Alive;
	float FallTime;
	float FallStartY;
};

struct ParticleInfoShared
{
	unsigned int SpawnEnable;
	Vector3 ScaleMin;
	Vector3 ScaleMax;
	Vector4 ColorMin;
	Vector4 ColorMax;
	int GravityEnable;
};

struct DissolveCBuffer
{
	Vector4 InLineColor;
	Vector4 OutLineColor;
	Vector4 CenterLineColor;
	float Burn;
	int Inverse;
	float InFilter;
	float OutFilter;
	float CenterFilter;
	Vector3 Dummy;
};
