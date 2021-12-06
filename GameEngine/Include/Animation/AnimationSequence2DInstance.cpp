#include "AnimationSequence2DInstance.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Component/SpriteComponent.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/Shader/Animation2DConstantBuffer.h"
#include "../Resource/Texture/Texture.h"

CAnimationSequence2DInstance::CAnimationSequence2DInstance()	:
	mScene(nullptr),
	mOwner(nullptr)
{
}

CAnimationSequence2DInstance::CAnimationSequence2DInstance(const CAnimationSequence2DInstance& anim)
{
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
	if (mOwner)
	{
		mOwner->SetTexture(0, 0, (int)eConstantBufferShaderTypeFlags::Pixel,
			mCurrentAnimation->mSequence->GetTexture()->GetName(), mCurrentAnimation->mSequence->GetTexture());
	}
}

bool CAnimationSequence2DInstance::Init()
{
	mCBuffer = mScene->GetResource()->GetAnimation2DCBuffer();

	return true;
}

void CAnimationSequence2DInstance::Update(float deltaTime)
{
	// ��ŸŸ�� ���ϱ�
	mCurrentAnimation->mTime += deltaTime * mCurrentAnimation->mPlayScale;

	bool bIsAnimEnd = false;

	// �� ������ �ð����� �� ���� ������ ���
	if (mCurrentAnimation->mTime >= mCurrentAnimation->mFrameTime)
	{
		// ������ �ð���ŭ ����
		mCurrentAnimation->mTime -= mCurrentAnimation->mFrameTime;

		// ������� ���
		if (mCurrentAnimation->mbIsReverse)
		{
			--mCurrentAnimation->mFrame;

			if (mCurrentAnimation->mFrame < 0)
			{
				bIsAnimEnd = true;
			}
		}
		// ������� ���
		else
		{
			++mCurrentAnimation->mFrame;

			if (mCurrentAnimation->mFrame == mCurrentAnimation->mSequence->GetFrameCount())
			{
				bIsAnimEnd = true;
			}
		}
	}

	// �ݹ� üũ
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

	// �ִϸ��̼� ���� ���
	if (bIsAnimEnd)
	{
		// �ݺ� �ִϸ��̼��� ���
		if (mCurrentAnimation->mbIsLoop)
		{
			// ������� ��� �� �� ����������
			if (mCurrentAnimation->mbIsReverse)
			{
				mCurrentAnimation->mFrame = mCurrentAnimation->mSequence->GetFrameCount();
			}
			else
			{
				mCurrentAnimation->mFrame = 0;
			}
		}
		// �ݺ��� �ƴ� ���
		else
		{
			if (mCurrentAnimation->mbIsReverse)
			{
				mCurrentAnimation->mFrame = 0;
			}
			else
			{
				mCurrentAnimation->mFrame = mCurrentAnimation->mSequence->GetFrameCount();
			}
		}

		// ���� �ݹ��� �ִ� ��� ����
		if (mCurrentAnimation->mEndCallBack)
		{
			mCurrentAnimation->mEndCallBack();
		}

		// �ݺ� �ִϸ��̼��� ��� �ݹ� ��� ȣ�� �ȵ� ���·� �ǵ���
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
	Vector2 startUV, endUV;

	Vector2 start = mCurrentAnimation->mSequence->GetFrameData(mCurrentAnimation->mFrame).Start;
	Vector2 frameSize = mCurrentAnimation->mSequence->GetFrameData(mCurrentAnimation->mFrame).Size;

	// ����ȭ -> ��ŸƮ ��ġ / ��ü ũ��
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

void CAnimationSequence2DInstance::AddAnimation(const std::string& sequenceName, const std::string& name, bool bIsLoop, const float playTime, const float playScale, bool bIsReverse)
{
	// �̹� �ش� �ִϸ��̼��� �����ϴ��� üũ
	CAnimationSequence2DData* anim = findAnimation(name);

	if (anim)
	{
		return;
	}

	// ������ ã�´�.
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

	// �ִϸ��̼� ������ ����
	anim = new CAnimationSequence2DData;

	anim->mSequence = sequence;
	anim->mbIsLoop = bIsLoop;
	anim->mPlayTime = playTime;
	anim->mPlayScale = playScale;
	anim->mbIsReverse = bIsReverse;
	anim->mFrameTime = playTime / sequence->GetFrameCount();

	// ���� �ִϸ��̼��� �ϳ��� ������ ���� �ִϸ��̼����� ����
	if (mMapAnimation.empty())
	{
		mCurrentAnimation = anim;

		// ���� SpriteComponent�� ���� ���
		if (mOwner)
		{
			// SpriteComponent�� Material�� �� �ִϸ��̼��� �ؽ��ĸ� ����Ѵ�.
			mOwner->SetTexture(0, 0, (int)eConstantBufferShaderTypeFlags::Pixel,
				anim->mSequence->GetTexture()->GetName(), anim->mSequence->GetTexture());
		}
	}

	mMapAnimation.insert(std::make_pair(name, anim));
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
	// ���� �ִϸ��̼����� ������ �ִϸ��̼��� ã�´�.
	mCurrentAnimation = findAnimation(name);

	if (!mCurrentAnimation)
	{
		assert(false);
		return;
	}

	mCurrentAnimation->mFrame = 0;
	mCurrentAnimation->mTime = 0.f;

	// �ش� �ִϸ��̼��� �ݹ��� ��� ȣ����� ���� ���·� �����Ѵ�.
	size_t size = mCurrentAnimation->mVecNotify.size();

	for (size_t i = 0; i < size; ++i)
	{
		mCurrentAnimation->mVecNotify[i]->bIsCalled = false;
	}

	if (mOwner)
	{
		// �ؽ��ĸ� ���� �ִϸ��̼��� �ؽ��ķ� �����Ѵ�. (�ټ��� �ؽ��ĸ� ����� ���� �ֱ� ����)
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
