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
	void Start();
	void Init();
	void Update(const float deltaTime);
	void PostUpdate(const float deltaTime);
	void SetTransformBuffer();
	void ComputeWolrdMatrix();
	CTransform* Clone();

public:
	void Save(FILE* fp);
	void Load(FILE* fp);

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

	void SetPivot(const Vector3& pivot)
	{
		mPivot = pivot;
	}
	
	void SetPivot(const float& x, const float& y, const float& z)
	{
		mPivot = Vector3(x, y, z);
	}

	void SetMeshSize(const Vector3& size)
	{
		mMeshSize = size;
	}

	void SetMeshSize(const float x, const float y, const float z)
	{
		mMeshSize = Vector3(x, y, z);
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

	void SetWorldScale(const Vector3& scale);
	void SetWorldScale(const float& x, const float& y, const float& z);
	void SetWorldRot(const Vector3& rot);
	void SetWorldRot(const float& x, const float& y, const float& z);
	void SetWorldRotX(const float& x);
	void SetWorldRotY(const float& y);
	void SetWorldRotZ(const float& z);
	void SetWorldPos(const Vector3& pos);
	void SetWorldPos(const float& x, const float& y, const float& z);

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

	void AddWorldScale(const Vector3& scale);
	void AddWorldScale(const float& x, const float& y, const float& z);
	void AddWorldRot(const Vector3 rot);
	void AddWorldRot(const float& x, const float& y, const float& z);
	void AddWorldRotX(const float& x);
	void AddWorldRotY(const float& y);
	void AddWorldRotZ(const float& z);
	void AddWorldPos(const Vector3& pos);
	void AddWorldPos(const float& x, const float& y, const float& z);

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

	Vector3 GetRelativeAxis(eAXIS axis)
	{
		return mRelativeAxis[axis];
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

	Vector3 GetPivot() const
	{
		return mPivot;
	}

	Vector3 GetMeshSize() const
	{
		return mMeshSize;
	}

	const Matrix& GetWorldMatrix() const
	{
		return mMatWorld;
	}

	Vector3 GetWorldAxis(eAXIS axis)
	{
		return mWorldAxis[axis];
	}

	bool GetInheritScale() const
	{
		return mbInheritScale;
	}

	bool GetInheritRotX() const
	{
		return mbInheritRotX;
	}

	bool GetInheritRotY() const
	{
		return mbInheritRotY;
	}

	bool GetInheritRotZ() const
	{
		return mbInheritRotZ;
	}

	bool GetInheritPosX() const
	{
		return mbInheritPosX;
	}

	bool GetInheritPosY() const
	{
		return mbInheritPosY;
	}

	bool GetInheritPosZ() const
	{
		return mbInheritPosZ;
	}

public:
	void SetInheritScaleValue();
	void SetInheritRotValue(bool bIsCurrent);
	void SetInheritPosValue();

	void SetInheritWorldScaleValue();
	void SetInheritWorldRotValue(bool bIsCurrent);
	void SetInheritWorldPosValue();

private:
	class CScene* mScene;
	class CGameObject* mObject;
	class CSceneComponent* mOwnerComponent;	// 이 트랜스폼의 소유자

	class CTransformConstantBuffer* mCBuffer; // 상수 버퍼 래퍼

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

private: // Pivot, Mesh
	Vector3 mPivot;
	Vector3 mMeshSize;

private: // Matrix
	Matrix mMatScale;
	Matrix mMatRot;
	Matrix mMatPos;
	Matrix mMatWorld;
};

