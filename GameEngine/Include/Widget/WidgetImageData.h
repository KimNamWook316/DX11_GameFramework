#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture/Texture.h"

class CWidgetImageData
{
public:
	CWidgetImageData();
	CWidgetImageData(const CWidgetImageData& data);
	~CWidgetImageData();

public:
	bool Init();
	void Start();
	void Update(float deltaTime);
	void SetShaderData();

public:
	CTexture* GetTexture() const
	{
		return mTexture;
	}

	Vector4 GetImageTint() const
	{
		return mTint;
	}

public:
	void SetTexture(CTexture* texture);
    bool SetTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const TCHAR* fullPath);
    bool SetTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);
    void SetTextureTint(const Vector4& tint);
    void SetTextureTint(const float r, const float g, const float b, const float a);
    void AddFrameData(const Vector2& start, const Vector2& size);
	void AddFrameDataFrameTexture(const int count);

public:
	void SetOwnerWidget(class CWidget* widget)
	{
		mOwnerWidget = widget;
	}

	void SetAnimationPlayTime(const float playTime)
	{
		mPlayTime = playTime;
	}

	void SetAnimationPlayScale(const float scale)
	{
		mPlayScale = scale;
	}

	void SetAnimationLoop(bool bLoop)
	{
		mbIsLoop = bLoop;
	}

	void SetAnimationReverse(bool bReverse)
	{
		mbIsReverse = bReverse;
	}

	void SetUseAnimation(bool bUse)
	{
		mbUseAnimation = bUse;
	}

	void SetPlay(bool bPlay)
	{
		mbPlay = bPlay;
	}

private:
	class CWidget* mOwnerWidget;
    CSharedPtr<CTexture> mTexture;
    Vector4 mTint;
    std::vector<AnimationFrameData> mVecFrameData;
	class CAnimation2DConstantBuffer* mAnimationCBuffer;
	bool mbUseAnimation;
	bool mbPlay;
	int mFrame;												// 몇 프레임 짜리인지	
	float mTime;											// 실행시간
	float mFrameTime;										// 프레임당 실행시간
	float mPlayTime;										// 총 실행시간
	float mPlayScale;										// 재생 배수
	bool mbIsLoop;											// 반복되는 애니메이션인지
	bool mbIsReverse;										// 역재생인지
};

