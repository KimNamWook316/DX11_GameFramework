#include "AnimationSequence2DInstance.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Component/SpriteComponent.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Resource/Texture/Texture.h"

CAnimationSequence2DInstance::CAnimationSequence2DInstance()	:
	mbPlay(true),
	mScene(nullptr),
	mOwner(nullptr),
	mCBuffer(nullptr),
	mCurrentAnimation(nullptr)
{
	SetTypeID<CAnimationSequence2DInstance>();
}

CAnimationSequence2DInstance::CAnimationSequence2DInstance(const CAnimationSequence2DInstance& anim)
{
	mbPlay = anim.mbPlay;

	auto iter = anim.mMapAnimation.begin();
	auto iterEnd = anim.mMapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		CAnimationSequence2DData* data = new CAnimationSequence2DData;

		data->mSequence = iter->second->mSequence;
		data->mName = iter->second->mName;
		data->mbIsLoop = iter->second->mbIsLoop;
		data->mPlayScale = iter->second->mPlayScale;
		data->mPlayTime = iter->second->mPlayTime;
		data->mbIsReverse = iter->second->mbIsReverse;

		if (anim.mCurrentAnimation->mName == iter->second->mName)
		{
			mCurrentAnimation = data;
		}

		mMapAnimation.insert(std::make_pair(iter->first, data));
	}
}

CAnimationSequence2DInstance::~CAnimationSequence2DInstance()
{
	auto iter = mMapAnimation.begin();
	auto iterEnd = mMapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CAnimationSequence2DInstance::Start()
{
	if (mOwner && mCurrentAnimation)
	{
		mOwner->SetTexture(0, 0, (int)eConstantBufferShaderTypeFlags::Pixel,
			mCurrentAnimation->mSequence->GetTexture()->GetName(), mCurrentAnimation->mSequence->GetTexture());
	}
}

bool CAnimationSequence2DInstance::Init()
{
	if (mScene)
	{
		mCBuffer = mScene->GetResource()->GetAnimation2DCBuffer();
	}

	return true;
}

void CAnimationSequence2DInstance::Update(float deltaTime)
{
	if (!mCurrentAnimation || !mbPlay || mCurrentAnimation->mSequence->GetFrameCount() == 0)
	{
		return;
	}

	// 한 프레임당 시간 실시간 계산 ( 프레임이 추가되면 Sequnce에 추가되기 때문에 갱신의 용이성때문에 이렇게 사용)
	mCurrentAnimation->mFrameTime = mCurrentAnimation->mPlayTime / mCurrentAnimation->mSequence->GetFrameCount();

	// 델타타임 더하기
	mCurrentAnimation->mTime += deltaTime * mCurrentAnimation->mPlayScale;

	bool bIsAnimEnd = false;

	// 한 프레임 시간보다 더 많이 지났을 경우
	if (mCurrentAnimation->mTime >= mCurrentAnimation->mFrameTime)
	{
		// 프레임 시간만큼 뺀다
		mCurrentAnimation->mTime -= mCurrentAnimation->mFrameTime;

		// 역재생일 경우
		if (mCurrentAnimation->mbIsReverse)
		{
			--mCurrentAnimation->mFrame;

			if (mCurrentAnimation->mFrame < 0)
			{
				bIsAnimEnd = true;
			}
		}
		// 정재생일 경우
		else
		{
			++mCurrentAnimation->mFrame;

			if (mCurrentAnimation->mFrame == mCurrentAnimation->mSequence->GetFrameCount())
			{
				bIsAnimEnd = true;
			}
		}
	}

	// 콜백 체크
	size_t size = mCurrentAnimation->mVecNotify.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (!mCurrentAnimation->mVecNotify[i]->bIsCalled &&
			mCurrentAnimation->mVecNotify[i]->Frame == mCurrentAnimation->mFrame)
		{
			mCurrentAnimation->mVecNotify[i]->bIsCalled = true;
			mCurrentAnimation->mVecNotify[i]->CallBack();
		}
	}

	// 애니메이션 끝난 경우
	if (bIsAnimEnd)
	{
		// 반복 애니메이션일 경우
		if (mCurrentAnimation->mbIsLoop)
		{
			// 역재생일 경우 맨 뒤 프레임으로
			if (mCurrentAnimation->mbIsReverse)
			{
				mCurrentAnimation->mFrame = mCurrentAnimation->mSequence->GetFrameCount() - 1;
			}
			else
			{
				mCurrentAnimation->mFrame = 0;
			}
		}
		// 반복이 아닌 경우
		else
		{
			if (mCurrentAnimation->mbIsReverse)
			{
				mCurrentAnimation->mFrame = 0;
			}
			else
			{
				mCurrentAnimation->mFrame = mCurrentAnimation->mSequence->GetFrameCount() - 1;
			}
		}

		// 엔드 콜백이 있는 경우 실행
		if (mCurrentAnimation->mEndCallBack)
		{
			mCurrentAnimation->mEndCallBack();
		}

		// 반복 애니메이션인 경우 콜백 모두 호출 안된 상태로 되돌림
		if (mCurrentAnimation->mbIsLoop)
		{
			size_t size = mCurrentAnimation->mVecNotify.size();

			for (size_t i = 0; i < size; ++i)
			{
				mCurrentAnimation->mVecNotify[i]->bIsCalled = false;
			}
		}
	}
}

void CAnimationSequence2DInstance::SetShader()
{
	if (!mCurrentAnimation)
	{
		return;
	}

	Vector2 startUV, endUV;

	Vector2 start = mCurrentAnimation->mSequence->GetFrameData(mCurrentAnimation->mFrame).Start;
	Vector2 frameSize = mCurrentAnimation->mSequence->GetFrameData(mCurrentAnimation->mFrame).Size;

	// 정규화 -> 스타트 위치 / 전체 크기
	startUV = start / Vector2((float)mCurrentAnimation->mSequence->GetTexture()->GetWidth(),
		(float)mCurrentAnimation->mSequence->GetTexture()->GetHeight());

	endUV = (start + frameSize) / Vector2((float)mCurrentAnimation->mSequence->GetTexture()->GetWidth(),
		(float)mCurrentAnimation->mSequence->GetTexture()->GetHeight());

	mCBuffer->SetAnimation2DType(mCurrentAnimation->mSequence->GetTexture()->GetImageType());
	mCBuffer->SetStartUV(startUV);
	mCBuffer->SetEndUV(endUV);
	mCBuffer->UpdateCBuffer();
}

void CAnimationSequence2DInstance::ResetShader()
{
}

CAnimationSequence2DInstance* CAnimationSequence2DInstance::Clone()
{
	return new CAnimationSequence2DInstance(*this);
}

void CAnimationSequence2DInstance::Save(const char* fullPath)
{
	FILE* fp = nullptr;

	fopen_s(&fp, fullPath, "wb");

	if (!fp)
	{
		assert(false);
		return;
	}
	
	Save(fp);

	fclose(fp);
}

void CAnimationSequence2DInstance::Save(FILE* fp)
{
	int mapSize = (int)mMapAnimation.size();
	fwrite(&mapSize, sizeof(int), 1, fp);

	auto iter = mMapAnimation.begin();
	auto iterEnd = mMapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		int length = (int)iter->first.length();
		fwrite(&length, sizeof(int), 1, fp);
		fwrite(iter->first.c_str(), sizeof(char), length, fp);
		iter->second->Save(fp);

		char sequenceFilename[MAX_PATH] = {};
		strcpy_s(sequenceFilename, iter->second->mSequenceName.c_str());
		strcat_s(sequenceFilename, ".sqc");
		iter->second->mSequence->Save(sequenceFilename, ANIMATION_PATH);
	}
	
	int length = (int)mCurrentAnimation->mName.size();
	fwrite(&length, sizeof(int), 1, fp);
	fwrite(mCurrentAnimation->mName.c_str(), sizeof(char), length, fp);

	fwrite(&mbPlay, sizeof(bool), 1, fp);
}

void CAnimationSequence2DInstance::Load(const char* fullPath)
{
	FILE* fp = nullptr;

	fopen_s(&fp, fullPath, "rb");

	if (!fp)
	{
		assert(false);
		return;
	}
	
	int mapSize = 0;
	fread(&mapSize, sizeof(int), 1, fp);

	for (int i = 0; i < mapSize; ++i)
	{
		CAnimationSequence2DData* data = new CAnimationSequence2DData;

		int length = 0;
		fread(&length, sizeof(int), 1, fp);
		char name[256] = {};
		fread(name, sizeof(char), size_t(length), fp);
		
		data->Load(fp);

		if (mScene)
		{
			char sequenceFullPath[MAX_PATH] = {};
			int fullPathLength = (int)strlen(fullPath);
			for (int i = fullPathLength - 1; i > 0; --i)
			{
				if (fullPath[i] == '\\')
				{
					strncpy_s(sequenceFullPath, fullPath, (size_t)i + (size_t)1);
					break;
				}
			}

			strcat_s(sequenceFullPath, data->mSequenceName.c_str());
			strcat_s(sequenceFullPath, ".sqc");
			
			mScene->GetResource()->LoadSequence2DFullPath(sequenceFullPath);
			data->mSequence = mScene->GetResource()->FindAnimationSequence2D(data->mSequenceName);
		}
		else
		{
			char sequenceFullPath[MAX_PATH] = {};
			int fullPathLength = (int)strlen(fullPath);
			for (int i = fullPathLength - 1; i > 0; --i)
			{
				if (fullPath[i] == '\\')
				{
					strncpy_s(sequenceFullPath, fullPath, (size_t)i + (size_t)1);
					break;
				}
			}

			strcat_s(sequenceFullPath, data->mSequenceName.c_str());
			strcat_s(sequenceFullPath, ".sqc");
			
			CResourceManager::GetInst()->LoadSequence2DFullPath(sequenceFullPath);
			data->mSequence = CResourceManager::GetInst()->FindAnimationSequece2D(data->mSequenceName);
		}

		mMapAnimation.insert(std::make_pair(name, data));
	}

	int length = 0;
	fread(&length, sizeof(int), 1, fp);
	char name[256] = {};
	fread(name, sizeof(char), size_t(length), fp);
	
	mCurrentAnimation = mMapAnimation.find(name)->second;

	fread(&mbPlay, sizeof(bool), 1, fp);

	if (mScene)
	{
		mCBuffer = mScene->GetResource()->GetAnimation2DCBuffer();
	}
	//Load(fp);

	fclose(fp);
}

void CAnimationSequence2DInstance::Load(FILE* fp)
{
	int mapSize = 0;
	fread(&mapSize, sizeof(int), 1, fp);

	for (int i = 0; i < mapSize; ++i)
	{
		CAnimationSequence2DData* data = new CAnimationSequence2DData;

		int length = 0;
		fread(&length, sizeof(int), 1, fp);
		char name[256] = {};
		fread(name, sizeof(char), size_t(length), fp);
		
		data->Load(fp);

		if (mScene)
		{
			char fileName[MAX_PATH] = {};
			strcpy_s(fileName, data->mSequenceName.c_str());
			strcat_s(fileName, ".sqc");
			
			mScene->GetResource()->LoadSequence2D(fileName);
			data->mSequence = mScene->GetResource()->FindAnimationSequence2D(data->mSequenceName);
		}
		else
		{
			char fileName[MAX_PATH] = {};
			strcpy_s(fileName, data->mSequenceName.c_str());
			strcat_s(fileName, ".sqc");

			CResourceManager::GetInst()->LoadSequence2D(fileName);
			data->mSequence = CResourceManager::GetInst()->FindAnimationSequece2D(data->mSequenceName);
		}

		mMapAnimation.insert(std::make_pair(name, data));
	}

	int length = 0;
	fread(&length, sizeof(int), 1, fp);
	char name[256] = {};
	fread(name, sizeof(char), size_t(length), fp);
	
	mCurrentAnimation = mMapAnimation.find(name)->second;

	fread(&mbPlay, sizeof(bool), 1, fp);

	if (mScene)
	{
		mCBuffer = mScene->GetResource()->GetAnimation2DCBuffer();
	}
}

void CAnimationSequence2DInstance::AddAnimation(const std::string& sequenceName, const std::string& name, bool bIsLoop, const float playTime, const float playScale, bool bIsReverse)
{
	// 이미 해당 애니메이션이 존재하는지 체크
	CAnimationSequence2DData* anim = findAnimation(name);

	if (anim)
	{
		return;
	}

	// 시퀀스 찾는다.
	CAnimationSequence2D* sequence = nullptr;

	if (mScene)
	{
		sequence = mScene->GetResource()->FindAnimationSequence2D(sequenceName);
	}
	else
	{
		sequence = CResourceManager::GetInst()->FindAnimationSequece2D(sequenceName);
	}

	if (!sequence)
	{
		assert(false);
		return;
	}

	// 애니메이션 데이터 생성
	anim = new CAnimationSequence2DData;

	anim->mSequenceName = sequence->GetName();
	anim->mSequence = sequence;
	anim->mName = name;
	anim->mbIsLoop = bIsLoop;
	anim->mPlayTime = playTime;
	anim->mPlayScale = playScale;
	anim->mbIsReverse = bIsReverse;
	anim->mFrameTime = playTime / sequence->GetFrameCount();

	// 현재 애니메이션이 하나도 없으면 현재 애니메이션으로 설정
	if (mMapAnimation.empty())
	{
		mCurrentAnimation = anim;

		// 주인 SpriteComponent가 있을 경우
		if (mOwner)
		{
			// SpriteComponent의 Material에 이 애니메이션의 텍스쳐를 등록한다.
			mOwner->SetTexture(0, 0, (int)eConstantBufferShaderTypeFlags::Pixel,
				anim->mSequence->GetTexture()->GetName(), anim->mSequence->GetTexture());
		}
	}

	mMapAnimation.insert(std::make_pair(name, anim));
}

void CAnimationSequence2DInstance::AddAnimation(const TCHAR* fileName, const std::string& pathName, 
	const std::string& name, bool bIsLoop, const float playTime, const float playScale, bool bIsReverse)
{
	CAnimationSequence2DData* anim = findAnimation(name);

	if (anim)
	{
		return;
	}

	char fileNameMultiByte[256] = {};
	int length = WideCharToMultiByte(CP_ACP, 0, fileName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, fileName, -1, fileNameMultiByte,length, 0, 0);

	CAnimationSequence2D* sequence = nullptr;

	if (mScene)
	{
		std::string sequenceName;
		mScene->GetResource()->LoadSequence2D(sequenceName, fileNameMultiByte);
		sequence = mScene->GetResource()->FindAnimationSequence2D(sequenceName);
	}
	else
	{
		std::string sequenceName;

		CResourceManager::GetInst()->LoadSequence2D(sequenceName, fileNameMultiByte);
		sequence = CResourceManager::GetInst()->FindAnimationSequece2D(sequenceName);
	}

	if (!sequence)
	{
		assert(false);
		return;
	}

	anim = new CAnimationSequence2DData;

	anim->mSequence = sequence;
	anim->mSequenceName = sequence->GetName();
	anim->mName = name;
	anim->mbIsLoop = bIsLoop;
	anim->mbIsReverse = bIsReverse;
	anim->mPlayTime = playTime;
	anim->mPlayScale = playScale;
	anim->mFrameTime = playTime / sequence->GetFrameCount();

	if (mMapAnimation.empty())
	{
		mCurrentAnimation = anim;
		if (mOwner)
		{
			mOwner->SetTexture(0, 0, (int)eConstantBufferShaderTypeFlags::Pixel, anim->mSequence->GetTexture()->GetName(),
				anim->mSequence->GetTexture());
		}
	}

	mMapAnimation.insert(std::make_pair(name, anim));
}

void CAnimationSequence2DInstance::DeleteAnimation(const std::string& name)
{
	auto iter = findAnimation(name);
	
	if (iter == nullptr)
	{
		return;
	}
	
	SAFE_DELETE(iter);
	mMapAnimation.erase(name);
}

void CAnimationSequence2DInstance::ChangeAnimation(const std::string& name)
{
	if (mCurrentAnimation->mName == name)
	{
		return;
	}

	mCurrentAnimation->mFrame = 0;
	mCurrentAnimation->mTime = 0.f;

	size_t size = mCurrentAnimation->mVecNotify.size();

	for (size_t i = 0; i < size; ++i)
	{
		mCurrentAnimation->mVecNotify[i]->bIsCalled = false;
	}

	mCurrentAnimation = findAnimation(name);

	if (!mCurrentAnimation)
	{
		assert(false);
		return;
	}

	mCurrentAnimation->mFrame = 0;
	mCurrentAnimation->mTime = 0.f;
	
	if (mOwner)
	{
		mOwner->SetTexture(0, 0, (int)eConstantBufferShaderTypeFlags::Pixel,
			mCurrentAnimation->mSequence->GetTexture()->GetName(), mCurrentAnimation->mSequence->GetTexture());
	}
}

bool CAnimationSequence2DInstance::CheckCurrentAnimation(const std::string& name)
{
	return mCurrentAnimation->mName == name;
}

void CAnimationSequence2DInstance::ReplayCurrentAnimation()
{
	mCurrentAnimation->Replay();
}

void CAnimationSequence2DInstance::SetPlayTime(const std::string& name, const float playTime)
{
	CAnimationSequence2DData* anim = findAnimation(name);

	if (!anim)
	{
		assert(false);
		return;
	}
	
	anim->mPlayTime = playTime;
	anim->mFrameTime = playTime / anim->mSequence->GetFrameCount();
}

void CAnimationSequence2DInstance::SetPlayScale(const std::string& name, const float playScale)
{
	CAnimationSequence2DData* anim = findAnimation(name);

	if (!anim)
	{
		assert(false);
		return;
	}

	anim->mPlayScale = playScale;
}

void CAnimationSequence2DInstance::SetLoop(const std::string& name, bool bLoop)
{
	CAnimationSequence2DData* anim = findAnimation(name);

	if (!anim)
	{
		assert(false);
		return;
	}

	anim->mbIsLoop = bLoop;
}

void CAnimationSequence2DInstance::SetReverse(const std::string& name, bool bReverse)
{
	CAnimationSequence2DData* anim = findAnimation(name);

	if (!anim)
	{
		assert(false);
		return;
	}

	anim->mbIsReverse = bReverse;
}

void CAnimationSequence2DInstance::SetCurrentAnimation(const std::string& name)
{
	// 현재 애니메이션으로 설정할 애니메이션을 찾는다.
	mCurrentAnimation = findAnimation(name);

	if (!mCurrentAnimation)
	{
		assert(false);
		return;
	}

	mCurrentAnimation->mFrame = 0;
	mCurrentAnimation->mTime = 0.f;

	// 해당 애니메이션의 콜백을 모두 호출되지 않은 상태로 변경한다.
	size_t size = mCurrentAnimation->mVecNotify.size();

	for (size_t i = 0; i < size; ++i)
	{
		mCurrentAnimation->mVecNotify[i]->bIsCalled = false;
	}

	if (mOwner)
	{
		// 텍스쳐를 현재 애니메이션의 텍스쳐로 변경한다. (다수의 텍스쳐를 사용할 수도 있기 때문)
		mOwner->SetTexture(0, 0, (int)eConstantBufferShaderTypeFlags::Pixel,
			mCurrentAnimation->mSequence->GetTexture()->GetName(), mCurrentAnimation->mSequence->GetTexture());
	}
}

CAnimationSequence2DData* CAnimationSequence2DInstance::findAnimation(const std::string& name)
{
	auto iter = mMapAnimation.find(name);

	if (iter == mMapAnimation.end())
	{
		return nullptr;
	}

	return iter->second;
}
