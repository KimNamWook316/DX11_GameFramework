#pragma once

#include "SceneComponent.h"

class CCameraComponent :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CCameraManager;

protected:
    CCameraComponent();
	CCameraComponent(const CCameraComponent& com);
    virtual ~CCameraComponent();

public:
	virtual bool Init() override;
	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void PostUpdate(float DeltaTime) override;
	virtual void PrevRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual CCameraComponent* Clone();

public:
	virtual void Save(FILE* fp) override;
	virtual void Load(FILE* fp) override;

public:
	Resolution GetResolution() const
	{
		return mRS;
	}

	eCameraType GetCameraType() const
	{
		return meCameraType;
	}

	Matrix GetViewMatrix() const
	{
		return mMatView;
	}

	Matrix GetProjMatrix() const
	{
		return mMatProj;
	}

	Vector2 GetLeftBottomPos() const
	{
		Vector2 LB;
		LB.x = GetWorldPos().x;
		LB.y = GetWorldPos().y;
		return LB;
	}

	const Vector3& GetScale() const
	{
		return mScale;
	}

	const Vector2& GetRatio() const
	{
		return mRatio;
	}

public:
	void SetCameraType(eCameraType type)
	{
		meCameraType = type;
	}

	void SetViewAngle(const float viewAngle)
	{
		mViewAngle = viewAngle;

		// 3D에서만 시야각에 영향을 받는다.
		if (meCameraType == eCameraType::Camera3D)
		{
			CreateProjectionMatrix();
		}
	}

	void SetDistance(const float dist)
	{
		mDistance = dist;
		CreateProjectionMatrix();
	}

	// 2D 전용
	void OnViewportCenter()
	{
		if (eCameraType::Camera2D == meCameraType)
		{
			float z = GetRelativePos().z;
			SetWorldPos(mRS.Width / -2.f, mRS.Height / -2.f, z);

			mRatio.x = 5.f;
			mRatio.y = 5.f;
		}
	}

	// 2D 전용
	void SetViewPortByRatio(const float x, const float y)
	{
		if (eCameraType::Camera2D == meCameraType)
		{
			float z = GetRelativePos().z;
			SetWorldPos(mRS.Width / -x, mRS.Height / -y, z);

			mRatio.x = x;
 			mRatio.y = y;
		}
	}

	void SetScale(const Vector3& scale)
	{
		mScale = scale;
	}

	void SetScale(const float x, const float y, const float z)
	{
		mScale = Vector3(x, y, z);
	}

private:
	void CreateProjectionMatrix();

protected:
    eCameraType meCameraType;
	Vector3 mScale;
    Matrix mMatView;
    Matrix mMatProj;
    float mViewAngle;
    float mDistance;
    Resolution mRS;
	Vector2 mRatio;
};

