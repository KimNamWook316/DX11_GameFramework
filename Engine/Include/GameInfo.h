#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }

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

struct Resolution {
	unsigned int Width;
	unsigned int Height;
};