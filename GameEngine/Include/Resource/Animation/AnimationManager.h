#pragma once

#include "AnimationSequence2D.h"

class CAnimationManager
{
	friend class CResourceManager;

private:
	CAnimationManager();
	~CAnimationManager();

public:
	bool Init();
	bool CreateAnimationSequence(const std::string& name, const std::string& textureName,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void AddFrame(const std::string& name, const Vector2& start, const Vector2& size);
	void AddFrame(const std::string& name, const float startX, const float startY,
		const float width, const float height);

	CAnimationSequence2D* FindSequence(const std::string& name);
	void ReleaseSequence(const std::string& name);

public:
	class CAnimation2DConstantBuffer* GetAnimation2DCBuffer() const
	{
		return mAnimation2DCBuffer;
	}

private:
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>> mMapSequence2D;
	class CAnimation2DConstantBuffer* mAnimation2DCBuffer;
};

