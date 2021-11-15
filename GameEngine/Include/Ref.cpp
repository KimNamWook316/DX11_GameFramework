#include "Ref.h"

CRef::CRef()
	: mRefCount(0)
	, mEnable(true)
	, mActive(true)
{
}

CRef::~CRef()
{
}
