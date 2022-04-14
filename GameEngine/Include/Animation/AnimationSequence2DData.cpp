#include "AnimationSequence2DData.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Scene/SceneManager.h"

CAnimationSequence2DData::CAnimationSequence2DData()	:
	mFrame(0),
	mTime(0.f),
	mFrameTime(0.f),
	mPlayTime(0.f),
	mPlayScale(1.f),
	mbIsLoop(false),
	mbIsReverse(false)
{
}

CAnimationSequence2DData::~CAnimationSequence2DData()
{
	auto iter = mVecNotify.begin();
	auto iterEnd = mVecNotify.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}
}

void CAnimationSequence2DData::Reset()
{
	// CallBack 호출되지 않은 상태로 변경
	auto iter = mVecNotify.begin();
	auto iterEnd = mVecNotify.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->bIsCalled = false;
	}
	
	// Play 관련 변수들 초기화
	mFrame = 0;
	mTime = 0.f;
}

void CAnimationSequence2DData::Save(FILE* fp)
{
	// mSequence->Save(fp);

	int length = (int)mName.length();
	fwrite(&length, sizeof(int), 1, fp);
	fwrite(mName.c_str(), sizeof(char), length, fp);
	fwrite(&mPlayTime, sizeof(float), 1, fp);
	fwrite(&mPlayScale, sizeof(float), 1, fp);
	fwrite(&mFrameTime, sizeof(float), 1, fp);
	fwrite(&mbIsLoop, sizeof(bool), 1, fp);
	fwrite(&mbIsReverse, sizeof(bool), 1, fp);

	length = (int)mSequenceName.length();
	fwrite(&length, sizeof(int), 1, fp);
	fwrite(mSequenceName.c_str(), sizeof(char), length, fp);
}

void CAnimationSequence2DData::Load(FILE* fp)
{
 //	CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetResource();
 //	
 //	std::string outName;
 //	if (!resource->LoadSequence2D(outName, fp))
 //	{
 //		assert(false);
 //		return false;
 //	}
 //	
 //	mSequence = resource->FindAnimationSequence2D(outName);

	int length = 0;
	char name[256] = {};
	fread(&length, sizeof(int), 1, fp);
	fread(name, sizeof(char), length, fp);
	mName = name;
	fread(&mPlayTime, sizeof(float), 1, fp);
	fread(&mPlayScale, sizeof(float), 1, fp);
	fread(&mFrameTime, sizeof(float), 1, fp);
	fread(&mbIsLoop, sizeof(bool), 1, fp);
	fread(&mbIsReverse, sizeof(bool), 1, fp);

	length = 0;
	char sequenceName[256] = {};
	fread(&length, sizeof(int), 1, fp);
	fread(sequenceName, sizeof(char), length, fp);
	mSequenceName = sequenceName;
}

CTexture* CAnimationSequence2DData::GetTexture() const
{
	return mSequence->GetTexture();
}

void CAnimationSequence2DData::DeleteNotify(const std::string& name)
{
	auto iter = mVecNotify.begin();
	auto iterEnd = mVecNotify.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == name)
		{
			SAFE_DELETE((*iter));
			mVecNotify.erase(iter);
			return;
		}
	}
}
