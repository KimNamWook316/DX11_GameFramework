#pragma once

#include "../../Ref.h"

// �ִϸ��̼��� ����� ��Ḧ ������ �ִ� Ŭ����
// �ؽ���(��Ʋ�� ����), ������ start, end, width, height ��
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

