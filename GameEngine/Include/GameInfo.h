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
	Vector3 MeshSize;
	// 16byte ������ ���� Dummy Data.
	Vector2 Dummy;
};