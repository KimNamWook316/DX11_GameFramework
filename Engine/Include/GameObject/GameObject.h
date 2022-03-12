#pragma once

#include "../Component/SceneComponent.h"
#include "../Component/ObjectComponent.h"

class CGameObject : public CRef
{
	friend class CScene;

protected:
	CGameObject();
	CGameObject(const CGameObject& obj);
	virtual ~CGameObject();

public:
	virtual void Enable(bool bEnable) override;
	virtual void Destroy();

public:
	bool IsExcludeFromSave() const
	{
		return mbExcludeFromSave;
	}

	class CScene* GetScene() const
	{
		return mScene;
	}

	CSceneComponent* GetRootSceneComponent() const
	{
		return mRootSceneComponent;
	}

	void AddSceneComponent(CSceneComponent* component)
	{
		mSceneComponentList.push_back(component);
	}

	class CComponent* FindComponent(const std::string& name);
	void DeleteComponent(class CComponent* comp);
	void DeleteComponent(const std::string& name);
	void GetAllSceneComponentsName(std::vector<FindComponentName>& outNames);
	void GetAllComponentsPointer(std::vector<CComponent*>& outPointers);
	void GetAllComponentsName(std::vector<FindComponentName>& outNames);

public:
	void SetScene(class CScene* scene);

	void SetRootSceneComponent(CSceneComponent* component)
	{
		mRootSceneComponent = component;
	}

	void SetLifeSpan(const float lifeSpan)
	{
		mLifeSpan = lifeSpan;
	}

	void ExcludeFromSave(bool bExclude)
	{
		mbExcludeFromSave = true;
	}

public:
	void AddChildGameObject(CGameObject* obj);
	void Move(const Vector3& endPos);	 // NavAgentComponent가 있을 경우 작동

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	void AddCollision();
	virtual void Render();
	virtual void PostRender();
	virtual CGameObject* Clone();

public:
	virtual void Save(FILE* fp);
	virtual void Save(const char* fileName, const std::string& pathName = OBJECT_PATH);
	virtual void SaveFullPath(const char* fullPath);
	virtual void Load(FILE* fp);
	virtual void Load(const char* fileName, const std::string& pathName = OBJECT_PATH);
	virtual void LoadFullPath(const char* fullPath);

public:
	template <typename T>
	T* CreateComponent(const std::string& name)
	{
		T* comp = new T;

		comp->SetName(name);
		comp->SetScene(mScene);
		comp->SetGameObject(this);

		if (!comp->Init())
		{
			SAFE_RELEASE(comp);
			return nullptr;
		}

		if (comp->GetComponentType() == eComponentType::OBJ_COMP)
		{
			mVecObjectComponent.push_back((class CObjectComponent*)comp);
		}
		else
		{
			mSceneComponentList.push_back((class CSceneComponent*)comp);

			if (!mRootSceneComponent)
			{
				mRootSceneComponent = (class CSceneComponent*)comp;
			}
		}
		
		// 씬컴포넌트는 계층 구조이므로 따로 저장
		return comp;
	}

	template <typename T>
	T* CreateComponentAddChild(const std::string& name)
	{
		T* comp = new T;

		comp->SetName(name);
		comp->SetScene(mScene);
		comp->SetGameObject(this);

		if (!comp->Init())
		{
			SAFE_RELEASE(comp);
			return nullptr;
		}

		if (comp->GetComponentType() == eComponentType::OBJ_COMP)
		{
			mVecObjectComponent.push_back((class CObjectComponent*)comp);
		}
		else
		{
			mSceneComponentList.push_back((class CSceneComponent*)comp);

			if (!mRootSceneComponent)
			{
				mRootSceneComponent = (class CSceneComponent*)comp;
			}
			else
			{
				mRootSceneComponent->AddChild((class CSceneComponent*)comp);
			}
		}
		
		return comp;
	}

	template <typename T>
	T* ReplaceComponent(CComponent* targetComp, const std::string& newCompName)
	{
		T* comp = new T;

		comp->SetName(newCompName);
		comp->SetScene(mScene);
		comp->SetGameObject(this);

		if (!comp->Init())
		{
			SAFE_RELEASE(comp);
			return nullptr;
		}

		if (eComponentType::OBJ_COMP == targetComp->GetComponentType())
		{
			size_t idx = 0;
			size_t size = mVecObjectComponent.size();
			
			for (; idx < size; ++idx)
			{
				if (targetComp == mVecObjectComponent[idx])
				{
					break;
				}
			}

			CObjectComponent* old = mVecObjectComponent[idx];
			mVecObjectComponent[idx] = (CObjectComponent*)comp;
			SAFE_RELEASE(old);
		}
		else
		{
			CSceneComponent* target = static_cast<CSceneComponent*>(targetComp);
			CSceneComponent* targetParent = target->mParent;
			std::vector<CComponent*> targetChild;
			target->GetAllChildPointer(targetChild);

			size_t size = targetChild.size();
			for (size_t i = 0; i < size; ++i)
			{
				static_cast<CSceneComponent*>(comp)->AddChild((CSceneComponent*)targetChild[i]);
			}

			if (target == mRootSceneComponent)
			{
				mRootSceneComponent = comp;
			}
			else
			{
				targetParent->AddChild(comp);
				targetParent->DeleteChild(target);
			}

			auto iter = mSceneComponentList.begin();
			auto iterEnd = mSceneComponentList.end();
			for (; iter != iterEnd; ++iter)
			{
				if (nullptr == (*iter))
				{
					mSceneComponentList.erase(iter);
					break;
				}
			}
		}
		return comp;
	}

	template <typename T>
	T* FindSceneComponentFromType()
	{
		auto iter = mSceneComponentList.begin();
		auto iterEnd = mSceneComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->CheckType<T>())
			{
				return (T*)(*iter);
			}
		}

		return nullptr;
	}

	template <typename T>
	T* FindObjectComponentFromType()
	{
		auto iter = mVecObjectComponent.begin();
		auto iterEnd = mVecObjectComponent.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->CheckType<T>())
			{
				return (T*)((*iter).Get());
			}
		}
		return nullptr;
	}

	template <typename T>
	T* LoadComponent()
	{
		T* component = new T;

		component->SetScene(mScene);
		component->SetGameObject(this);

		if (component->GetComponentType() == eComponentType::OBJ_COMP)
		{
			mVecObjectComponent.push_back((class CObjectComponent*)component);
		}
		else
		{
			mSceneComponentList.push_back((class CSceneComponent*)component);
			if (!mRootSceneComponent)
			{
				mRootSceneComponent = (class CSceneComponent*)component;
			}
		}

		return component;
	}

public:
	void SetBoolInheritScale(bool Inherit)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetBoolInheritScale(Inherit);
	}

	void SetBoolInheritRotX(bool Inherit)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetBoolInheritRotX(Inherit);
	}

	void SetBoolInheritRotY(bool Inherit)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetBoolInheritRotY(Inherit);
	}

	void SetBoolInheritRotZ(bool Inherit)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetBoolInheritRotZ(Inherit);
	}

	void SetBoolInheritPosX(bool Inherit)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetBoolInheritPosX(Inherit);
	}

	void SetBoolInheritPosY(bool Inherit)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetBoolInheritPosY(Inherit);
	}

	void SetBoolInheritPosZ(bool Inherit)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetBoolInheritPosZ(Inherit);
	}

	void SetInheritScaleValue()
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetInheritScaleValue();
	}

	void SetInheritRotValue(bool Current)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetInheritRotValue(Current);
	}

	void SetInheritPosValue()
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetInheritPosValue();
	}

	void SetInheritWorldScaleValue()
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetInheritWorldScaleValue();
	}

	void SetInheritWorldRotValue(bool Current)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetInheritWorldRotValue(Current);
	}

public:
	Vector3 GetRelativeScale()	const
	{
		if (!mRootSceneComponent)
			return Vector3();

		return mRootSceneComponent->GetRelativeScale();
	}

	Vector3 GetRelativeRot()	const
	{
		if (!mRootSceneComponent)
			return Vector3();

		return mRootSceneComponent->GetRelativeRot();
	}

	Vector3 GetRelativePos()	const
	{
		if (!mRootSceneComponent)
			return Vector3();

		return mRootSceneComponent->GetRelativePos();
	}

	Vector3 GetRelativeAxis(eAXIS Axis)
	{
		if (!mRootSceneComponent)
			return Vector3();

		return mRootSceneComponent->GetRelativeAxis(Axis);
	}

public:
	void SetRelativeScale(const Vector3& Scale)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetRelativeScale(Scale);
	}

	void SetRelativeScale(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetRelativeScale(x, y, z);
	}

	void SetRelativeRot(const Vector3& Rot)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetRelativeRot(Rot);
	}

	void SetRelativeRot(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetRelativeRot(x, y, z);
	}

	void SetRelativeRotX(float x)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetRelativeRotX(x);
	}

	void SetRelativeRotY(float y)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetRelativeRotY(y);
	}

	void SetRelativeRotZ(float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetRelativeRotZ(z);
	}

	void SetRelativePos(const Vector3& Pos)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetRelativePos(Pos);
	}

	void SetRelativePos(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetRelativePos(x, y, z);
	}

	void AddRelativeScale(const Vector3& Scale)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddRelativeScale(Scale);
	}

	void AddRelativeScale(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddRelativeScale(x, y, z);
	}

	void AddRelativeRot(const Vector3& Rot)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddRelativeRot(Rot);
	}

	void AddRelativeRot(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddRelativeRot(x, y, z);
	}

	void AddRelativeRotX(float x)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddRelativeRotX(x);
	}

	void AddRelativeRotY(float y)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddRelativeRotY(y);
	}

	void AddRelativeRotZ(float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddRelativeRotZ(z);
	}

	void AddRelativePos(const Vector3& Pos)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddRelativePos(Pos);
	}

	void AddRelativePos(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddRelativePos(x, y, z);
	}

public:
	Vector3 GetWorldScale()	const
	{
		if (!mRootSceneComponent)
			return Vector3();

		return mRootSceneComponent->GetWorldScale();
	}

	Vector3 GetWorldRot()	const
	{
		if (!mRootSceneComponent)
			return Vector3();

		return mRootSceneComponent->GetWorldRot();
	}

	Vector3 GetWorldPos()	const
	{
		if (!mRootSceneComponent)
			return Vector3();

		return mRootSceneComponent->GetWorldPos();
	}

	Vector3 GetPivot()	const
	{
		if (!mRootSceneComponent)
			return Vector3();

		return mRootSceneComponent->GetPivot();
	}

	Vector3 GetMeshSize()	const
	{
		if (!mRootSceneComponent)
			return Vector3();

		return mRootSceneComponent->GetMeshSize();
	}

	const Matrix& GetWorldMatrix()	const
	{
		if (!mRootSceneComponent)
			return Matrix();

		return mRootSceneComponent->GetWorldMatrix();
	}

	Vector3 GetWorldAxis(eAXIS Axis)
	{
		if (!mRootSceneComponent)
			return Vector3();

		return mRootSceneComponent->GetWorldAxis(Axis);
	}

public:
	void SetPivot(const Vector3& Pivot)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetPivot(Pivot);
	}

	void SetPivot(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetPivot(x, y, z);
	}

	void SetMeshSize(const Vector3& Size)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetMeshSize(Size);
	}

	void SetMeshSize(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetMeshSize(x, y, z);
	}

public:
	void SetWorldScale(const Vector3& Scale)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetWorldScale(Scale);
	}

	void SetWorldScale(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetWorldScale(x, y, z);
	}

	void SetWorldRot(const Vector3& Rot)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetWorldRot(Rot);
	}

	void SetWorldRot(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetWorldRot(x, y, z);
	}

	void SetWorldRotX(float x)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetWorldRotX(x);
	}

	void SetWorldRotY(float y)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetWorldRotY(y);
	}

	void SetWorldRotZ(float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetWorldRotZ(z);
	}

	void SetWorldPos(const Vector3& Pos)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetWorldPos(Pos);
	}

	void SetWorldPos(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->SetWorldPos(x, y, z);
	}

	void AddWorldScale(const Vector3& Scale)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddWorldScale(Scale);
	}

	void AddWorldScale(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddWorldScale(x, y, z);
	}

	void AddWorldRot(const Vector3& Rot)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddWorldRot(Rot);
	}

	void AddWorldRot(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddWorldRot(x, y, z);
	}

	void AddWorldRotX(float x)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddWorldRotX(x);
	}

	void AddWorldRotY(float y)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddWorldRotY(y);
	}

	void AddWorldRotZ(float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddWorldRotZ(z);
	}

	void AddWorldPos(const Vector3& Pos)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddWorldPos(Pos);
	}

	void AddWorldPos(float x, float y, float z)
	{
		if (!mRootSceneComponent)
			return;

		mRootSceneComponent->AddWorldPos(x, y, z);
	}

protected:
	class CScene* mScene;
	CSharedPtr<CSceneComponent> mRootSceneComponent; // 씬 컴포넌트는 계층 구조로 관리
	std::vector<CSharedPtr<CObjectComponent>> mVecObjectComponent; // 삭제, 삽입 많지 않지만, 각 컴포넌트에 접근할 일 많음
	std::list<CSceneComponent*> mSceneComponentList; // 탐색을 위해 씬 컴포넌트를 리스트로 들고 있다.

	CGameObject* mParent;	// 부모 게임 오브젝트
	std::vector<CSharedPtr<CGameObject>> mVecChildGameObj; // 자식 게임 오브젝트
	float mLifeSpan;
	bool mbExcludeFromSave; // 씬에서 저장 시 이 플래그가 true이면 저장에서 제외됨
};

