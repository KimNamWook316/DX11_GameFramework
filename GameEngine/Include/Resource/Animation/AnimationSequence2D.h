#pragma once

#include "../../Ref.h"

// 애니메이션을 재생할 재료를 가지고 있는 클래스
// 텍스쳐(아틀라스 형태), 프레임 start, end, width, height 등
class CAnimationSequence2D : public CRef
{
	friend class CAnimationManager;

protected:
	CAnimationSequence2D();
	~CAnimationSequence2D();

public:
	bool Init(class CTexture* texture);
	bool Init(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH);
	void AddFrame(const Vector2& startPos, const Vector2& size);
	void AddFrame(const float startX, const float startY, const float width, const float height);
	void DeleteFrame(const int idx);
	void ClearFrame();

public:
	void SetScene(class CScene* scene);

public:
	class CTexture* GetTexture() const
	{
		return mTexture;
	}

	const AnimationFrameData& GetFrameData(const int index) const
	{
		return mVecFrameData[index];
	}

	int GetFrameCount() const
	{
		return (int)mVecFrameData.size();
	}

private:
	class CScene* mScene;
	CSharedPtr<class CTexture> mTexture;
	std::vector<AnimationFrameData> mVecFrameData;
};

