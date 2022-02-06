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

// ��ġ ������ �� �������� ���� Vertex. �ٸ� ������ ���� Vertex�� ������ ���� ������ �� �ִ�.
// ���� ������ �ð������ ��� �ո����� ����Ѵ�. ���߿� Back-Face Culling�Ͽ�, ������ �ʴ� �޸� ���������� �ʴ� ������� ����Ѵ�.
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

// �ؽ��� ������ UV��ǥ�� ������ �ִ� Vertex
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

// ���� ������ ���� ����
struct VertexBuffer
{
	ID3D11Buffer*	Buffer;
	int				Size;	// Vertex�ϳ��� ũ��
	int				Count;	// Vertex�� �� ����
	
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

// ������ �����ϴ� Index�� ���� ����
struct IndexBuffer
{
	ID3D11Buffer*	Buffer;
	int				Size;	// Index �ϳ��� ũ��
	int				Count;	// Index ����
	DXGI_FORMAT		Fmt;	// ����� �� �ʿ��� Format�� ��� �ִ�.

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

// �޽� ������ ��� �ִ� �����̳�
struct MeshContainer
{
	// ���ؽ� ����
	VertexBuffer	VB;

	// �޽��� Subset���� ������ �ִ� ���, �ε����� �ɰ��� �����Ѵ�.
	// Subset���� ������ ������, 3D���ҽ��� ��� Material(����)�� �ٸ��� ������ ��찡 �ٺ��ϱ� �����̴�.
	// ������ VertexBuffer�� �׳� ���� ������ ���̰�, ������ �̾��ֱ� ���ؼ� Index�� �ʿ��ϱ� ������ �ε��� ���۸� �ɰ��� �����Ѵ�.
	std::vector<IndexBuffer>	vecIB;

	// ������ �̾��ִ� ����̴�.
	D3D11_PRIMITIVE_TOPOLOGY	Primitive;	
};

// Shader cbuffer�� �����ϴ� ���� ������ ��� �ִ� ����ü
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
	// 16byte ������ ���� Dummy Data.
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
	eCollisionChannel Channel; // �� ä���� � ä�ο� �����ִ���
	bool bCollisionEnable;

	std::vector<eCollisionInteraction> vecInteraction; // �� ä�ο� ���� �浹�ϴ��� �ƴ��� ( �� ä�μ���ŭ ������)
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
	Vector2 Length; // Collider Box�� �ʺ�, ������ ����
	Vector2 Min;	// ���� ���� ��ǥ��
	Vector3 Max;	// ���� ���� ��ǥ��
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
	unsigned char* Pixel; // �̹��� �迭���·� ����
	unsigned int Width;
	unsigned int Height;
	ePixelCollisionType Type;
	Vector2 Center;
	Vector2 Min;
	Vector2 Max;
	Box2DInfo Box;
	unsigned char Color[4]; // 4byte r, g, b, a �迭
	ID3D11ShaderResourceView* SRV;
	int RefCount; // ������ �ؽ�ó�� ���� �� ������ �ʿ䰡 ���� ������, Ref Check�ؼ� ����

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
	unsigned int SpawnEnable; // ���� ��ƼŬ ���� ����
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
	float AccTime; // ���� �ð�
	Vector2 Resolution;
	Vector2 NoiseResolution; // ���� �Լ��� ���� ������ �ؽ��� �ػ�
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
