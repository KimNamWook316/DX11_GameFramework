#pragma once

#include "../GameInfo.h"

class CTransform
{
	friend class CSceneComponent;

private:
	CTransform();
	CTransform(const CTransform& trans);
	~CTransform();

public:
	void Init();
	CTransform* Clone();

public: // setter
	void SetBoolInheritScale(bool bInherit)
	{
		mbInheritScale = bInherit;
	}

	void SetBoolInheritRotX(bool bInherit)
	{
		mbInheritRotX = bInherit;
	}

	void SetBoolInheritRotY(bool bInherit)
	{
		mbInheritRotY = bInherit;
	}

	void SetBoolInheritRotZ(bool bInherit)
	{
		mbInheritRotZ = bInherit;
	}

	void SetBoolInheritPosX(bool bInherit)
	{
		mbInheritPosX = bInherit;
	}

	void SetBoolInheritPosY(bool bInherit)
	{
		mbInheritPosY = bInherit;
	}

	void SetBoolInheritPosZ(bool bInherit)
	{
		mbInheritPosZ = bInherit;
	}

	void SetRelativeScale(const Vector3& scale);
	void SetRelativeScale(const float& x, const float& y, const float& z);
	void SetRelativeRot(const Vector3& rot);
	void SetRelativeRot(const float& x, const float& y, const float& z);
	void SetRelativeRotX(const float& x);
	void SetRelativeRotY(const float& y);
	void SetRelativeRotZ(const float& z);
	void SetRelativePos(const Vector3& pos);
	void SetRelativePos(const float& x, const float& y, const float& z);

public:
	void AddRelativeScale(const Vector3& scale);
	void AddRelativeScale(const float& x, const float& y, const float& z);
	void AddRelativeRot(const Vector3 rot);
	void AddRelativeRot(const float& x, const float& y, const float& z);
	void AddRelativeRotX(const float& x);
	void AddRelativeRotY(const float& y);
	void AddRelativeRotZ(const float& z);
	void AddRelativePos(const Vector3& pos);
	void AddRelativePos(const float& x, const float& y, const float& z);


public: // getter
	Vector3 GetRelativeScale() const
	{
		return mRelativeScale;
	}

	Vector3 GetRelativeRot() const
	{
		return mRelativeRot;
	}

	Vector3 GetRelativePos() const
	{
		return mRelativePos;
	}

	Vector3 GetWorldScale() const
	{
		return mWorldScale;
	}

	Vector3 GetWorldRot() const
	{
		return mWorldRot;
	}

	Vector3 GetWorldPos() const
	{
		return mWorldPos;
	}

public:
	void SetInheritScaleValue();
	void SetInheritRotValue(bool bIsCurrent);
	void SetInheritPosValue();

private:
	class CScene* mScene;
	class CGameObject* mObject;
	class CSceneComponent* mOwnerComponent;

	CTransform* mParentTransform;
	std::vector<CTransform*> mVecChildTransform;

private:
	// 부모 Transform에 의해 영향을 받는지 판단하는 변수
	bool mbInheritScale;
	bool mbInheritRotX;
	bool mbInheritRotY;
	bool mbInheritRotZ;
	bool mbInheritPosX;
	bool mbInheritPosY;
	bool mbInheritPosZ;

	// Update 여부를 판단하는 변수 ( 필요 없는 경우 연산하지 않기 위해 )
	bool mbUpdateScale;
	bool mbUpdateRot;
	bool mbUpdatePos;

private: // Relative
	Vector3 mRelativeScale;
	Vector3 mRelativePos;
	Vector3 mRelativeRot;
	Vector3 mRelativeAxis[AXIS_MAX];

private: // World
	Vector3 mWorldScale;
	Vector3 mWorldPos;
	Vector3 mWorldRot;
	Vector3 mWorldAxis[AXIS_MAX];
};

