#pragma once

#include "Component.h"
#include "Transform.h"

// 씬에서 보여질 수 있는 컴포넌트
class CSceneComponent : public CComponent
{
	friend class CGameObject;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& com);
	virtual ~CSceneComponent();
	
public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CSceneComponent* Clone();

public:
	bool GetIsRender()	const
	{
		return mbIsRender;
	}

public:
	// GameObject의 SceneComponent List에 자신을 추가
	void SetThisToGameObject(class CGameObject* obj);

public:
	// 자신을 갖고 있는 씬과 오브젝트를 등록하는 setter
	virtual void SetScene(class CScene* scene);
	virtual void SetGameObject(class CGameObject* obj);

public:
	void AddChild(CSceneComponent* child);
	bool DeleteChild(CSceneComponent* deleteTarget);
	bool DeleteChild(std::string& deleteTargetName);
	CSceneComponent* FindComponent(const std::string& name);

public:	// =============== 저주받은 Transform 영역 ===============
	void SetBoolInheritScale(bool Inherit)
	{
		mTransform->SetBoolInheritScale(Inherit);
	}

	void SetBoolInheritRotX(bool Inherit)
	{
		mTransform->SetBoolInheritRotX(Inherit);
	}

	void SetBoolInheritRotY(bool Inherit)
	{
		mTransform->SetBoolInheritRotY(Inherit);
	}

	void SetBoolInheritRotZ(bool Inherit)
	{
		mTransform->SetBoolInheritRotZ(Inherit);
	}

	void SetBoolInheritPosX(bool Inherit)
	{
		mTransform->SetBoolInheritPosX(Inherit);
	}

	void SetBoolInheritPosY(bool Inherit)
	{
		mTransform->SetBoolInheritPosY(Inherit);
	}

	void SetBoolInheritPosZ(bool Inherit)
	{
		mTransform->SetBoolInheritPosZ(Inherit);
	}

	void SetInheritScaleValue()
	{
		mTransform->SetInheritScaleValue();
	}

	void SetInheritRotValue(bool Current)
	{
		mTransform->SetInheritRotValue(Current);
	}

	void SetInheritPosValue()
	{
		mTransform->SetInheritPosValue();
	}

	void SetInheritWorldScaleValue()
	{
		mTransform->SetInheritWorldScaleValue();
	}

	void SetInheritWorldRotValue(bool Current)
	{
		mTransform->SetInheritWorldRotValue(Current);
	}

public:
	Vector3 GetRelativeScale()	const
	{
		return mTransform->GetRelativeScale();
	}

	Vector3 GetRelativeRot()	const
	{
		return mTransform->GetRelativeRot();
	}

	Vector3 GetRelativePos()	const
	{
		return mTransform->GetRelativePos();
	}

	Vector3 GetRelativeAxis(eAXIS Axis)
	{
		return mTransform->GetRelativeAxis(Axis);
	}

public:
	void SetRelativeScale(const Vector3& Scale)
	{
		mTransform->SetRelativeScale(Scale);
	}

	void SetRelativeScale(float x, float y, float z)
	{
		mTransform->SetRelativeScale(x, y, z);
	}

	void SetRelativeRot(const Vector3& Rot)
	{
		mTransform->SetRelativeRot(Rot);
	}

	void SetRelativeRot(float x, float y, float z)
	{
		mTransform->SetRelativeRot(x, y, z);
	}

	void SetRelativeRotX(float x)
	{
		mTransform->SetRelativeRotX(x);
	}

	void SetRelativeRotY(float y)
	{
		mTransform->SetRelativeRotY(y);
	}

	void SetRelativeRotZ(float z)
	{
		mTransform->SetRelativeRotZ(z);
	}

	void SetRelativePos(const Vector3& Pos)
	{
		mTransform->SetRelativePos(Pos);
	}

	void SetRelativePos(float x, float y, float z)
	{
		mTransform->SetRelativePos(x, y, z);
	}

	void AddRelativeScale(const Vector3& Scale)
	{
		mTransform->AddRelativeScale(Scale);
	}

	void AddRelativeScale(float x, float y, float z)
	{
		mTransform->AddRelativeScale(x, y, z);
	}

	void AddRelativeRot(const Vector3& Rot)
	{
		mTransform->AddRelativeRot(Rot);
	}

	void AddRelativeRot(float x, float y, float z)
	{
		mTransform->AddRelativeRot(x, y, z);
	}

	void AddRelativeRotX(float x)
	{
		mTransform->AddRelativeRotX(x);
	}

	void AddRelativeRotY(float y)
	{
		mTransform->AddRelativeRotY(y);
	}

	void AddRelativeRotZ(float z)
	{
		mTransform->AddRelativeRotZ(z);
	}

	void AddRelativePos(const Vector3& Pos)
	{
		mTransform->AddRelativePos(Pos);
	}

	void AddRelativePos(float x, float y, float z)
	{
		mTransform->AddRelativePos(x, y, z);
	}

public:
	Vector3 GetWorldScale()	const
	{
		return mTransform->GetWorldScale();
	}

	Vector3 GetWorldRot()	const
	{
		return mTransform->GetWorldRot();
	}

	Vector3 GetWorldPos()	const
	{
		return mTransform->GetWorldPos();
	}

	Vector3 GetPivot()	const
	{
		return mTransform->GetPivot();
	}

	Vector3 GetMeshSize()	const
	{
		return mTransform->GetMeshSize();
	}

	const Matrix& GetWorldMatrix()	const
	{
		return mTransform->GetWorldMatrix();
	}

	Vector3 GetWorldAxis(eAXIS Axis)
	{
		return mTransform->GetWorldAxis(Axis);
	}

	std::string GetLayerName() const
	{
		return mLayerName;
	}

public:
	void SetPivot(const Vector3& Pivot)
	{
		mTransform->SetPivot(Pivot);
	}

	void SetPivot(float x, float y, float z)
	{
		mTransform->SetPivot(x, y, z);
	}

	void SetMeshSize(const Vector3& Size)
	{
		mTransform->SetMeshSize(Size);
	}

	void SetMeshSize(float x, float y, float z)
	{
		mTransform->SetMeshSize(x, y, z);
	}

	void SetLayerName(const std::string& name)
	{
		mLayerName = name;
	}

public:
	void SetWorldScale(const Vector3& Scale)
	{
		mTransform->SetWorldScale(Scale);
	}

	void SetWorldScale(float x, float y, float z)
	{
		mTransform->SetWorldScale(x, y, z);
	}

	void SetWorldRot(const Vector3& Rot)
	{
		mTransform->SetWorldRot(Rot);
	}

	void SetWorldRot(float x, float y, float z)
	{
		mTransform->SetWorldRot(x, y, z);
	}

	void SetWorldRotX(float x)
	{
		mTransform->SetWorldRotX(x);
	}

	void SetWorldRotY(float y)
	{
		mTransform->SetWorldRotY(y);
	}

	void SetWorldRotZ(float z)
	{
		mTransform->SetWorldRotZ(z);
	}

	void SetWorldPos(const Vector3& Pos)
	{
		mTransform->SetWorldPos(Pos);
	}

	void SetWorldPos(float x, float y, float z)
	{
		mTransform->SetWorldPos(x, y, z);
	}

	void AddWorldScale(const Vector3& Scale)
	{
		mTransform->AddWorldScale(Scale);
	}

	void AddWorldScale(float x, float y, float z)
	{
		mTransform->AddWorldScale(x, y, z);
	}

	void AddWorldRot(const Vector3& Rot)
	{
		mTransform->AddWorldRot(Rot);
	}

	void AddWorldRot(float x, float y, float z)
	{
		mTransform->AddWorldRot(x, y, z);
	}

	void AddWorldRotX(float x)
	{
		mTransform->AddWorldRotX(x);
	}

	void AddWorldRotY(float y)
	{
		mTransform->AddWorldRotY(y);
	}

	void AddWorldRotZ(float z)
	{
		mTransform->AddWorldRotZ(z);
	}

	void AddWorldPos(const Vector3& Pos)
	{
		mTransform->AddWorldPos(Pos);
	}

	void AddWorldPos(float x, float y, float z)
	{
		mTransform->AddWorldPos(x, y, z);
	}

protected:
	bool mbIsRender; // 렌더 되어야 하는 컴포넌트인지
	CTransform* mTransform;	// 씬에 보여지는 컴포넌트이므로 위치 정보는 무조건 있어야 한다.
	CSceneComponent* mParent; // 상위 컴포넌트
	std::vector<CSharedPtr<CSceneComponent>> mVecChild; // 자식 컴포넌트
	std::string mLayerName;
};

