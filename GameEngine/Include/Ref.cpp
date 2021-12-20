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

void CRef::Save(FILE* fp)
{
	int length = (int)mName.length();
	fwrite(&length, sizeof(int), 1, fp);
	fwrite(mName.c_str(), sizeof(char), 1, fp);
	fwrite(&mEnable, sizeof(bool), 1, fp);
	fwrite(&mActive, sizeof(bool), 1, fp);
	fwrite(&mTypeID, sizeof(size_t), 1, fp);
}

void CRef::Load(FILE* fp)
{
	int length = 0;
	fread(&length, sizeof(int), 1, fp);

	char name[256] = {};

	fread(name, sizeof(char), 1, fp);
	mName = name;
	fread(&mEnable, sizeof(bool), 1, fp);
	fread(&mActive, sizeof(bool), 1, fp);
	fread(&mTypeID, sizeof(size_t), 1, fp);
}
