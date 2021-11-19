#include "Ref.h"

CRef::CRef()
	: mRefCount(0)
	, mEnable(true)
	, mActive(true)
	, mTypeID(0)
{
}

CRef::~CRef()
{
}
