#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <crtdbg.h>
#include <functional>
#include <string>
#include <math.h>
#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"

#define ROOT_PATH		"Root"
#define SHADER_PATH		"Shader"

#define SAFE_DELETE(p)  if(p) { delete p; p = nullptr; }
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
	Vector3 MeshSize;
	// 16byte 정렬을 위한 Dummy Data.
	Vector2 Dummy;
};