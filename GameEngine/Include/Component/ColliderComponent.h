#pragma once

#include "SceneComponent.h"

class CColliderComponent :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CCollision;

protected:
    CColliderComponent();
    CColliderComponent(const CColliderComponent& com);
    virtual ~CColliderComponent();

public:
    virtual bool Init() override;
    virtual void Start() override;
    virtual void Update(float deltaTime) override;
    virtual void PostUpdate(float deltaTime) override;
    virtual void CheckCollision() override;
    virtual void PrevRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual CColliderComponent* Clone();
    virtual void Save(FILE* fp) override;
    virtual void Load(FILE* fp) override;

    virtual bool DoCollide(CColliderComponent* Dest) = 0;
    virtual bool DoCollideMouse(const Vector2& mousePos) = 0;

public:
    void CheckPrevCollisionSection();
    void AddPrevCollision(CColliderComponent* collider);
    void DeletePrevCollision(CColliderComponent* collider);
    bool EmptyPrevCollision();
    bool IsExistInPrevCollision(CColliderComponent* collider);
    bool IsExistInCurrentCollision(CColliderComponent* collider);
    void AddCurrentFrameCollision(CColliderComponent* collider);
    void CallCollisionCallBack(eCollisionState state);
    void CallCollisionMouseCallBack(eCollisionState state);
    void ClearFrame();

public:
    template <typename T>
    void AddCollisionCallBack(eCollisionState state, T* obj, void(T::* func)(const CollisionResult&))
    {
        mCollisionCallBack[state] = std::bind(obj, std::placeholders::_1);
    }

    template <typename T>
    void AddCollisionMouseCallBack(eCollisionState state, T* obj, void(T::* func)(const CollisionResult&))
    {
        mCollisionMouseCallBack[state] = std::bind(obj, std::placeholders::_1);
    }

public:
    eColliderType GetColliderType() const
    {
        return meColliderType;
    }

    Vector3 GetMinPos() const
    {
        return mMinPos;
    }

    Vector3 GetMaxPos() const
    {
        return mMaxPos;
    }

    CollisionResult GetCollisionResult() const
    {
        return mResult;
    }

    CollisionResult GetCollisionMouseResult() const
    {
        return mMouseResult;
    }

    CollisionProfile* GetCollisionProfile() const
    {
        return mProfile;
    }

    void AddSectionIndex(const int idx)
    {
        mVecSectionIndex.push_back(idx);
    }

    void CheckCurrentSection()
    {
        mbCheckCurrentSection = true;
    }

    bool GetCheckCurrentSection() const
    {
        return mbCheckCurrentSection;
    }

public:
    void SetColiderType(eColliderType type)
    {
        meColliderType = type;
    }

    void SetOffset(const Vector3& offset)
    {
        mOffset = offset;
    }

    void SetOffset(const float x, const float y, const float z)
    {
        mOffset = Vector3(x, y, z);
    }

    void SetCollisionProfile(const std::string& name);

protected:
    eColliderType meColliderType;
    CollisionProfile* mProfile;
    Vector3 mOffset;
    Vector3 mMinPos;
    Vector3 mMaxPos;
    std::vector<int> mVecSectionIndex;                      // �� �浹ü�� �����ִ� ���� ����Ʈ
    std::list<CColliderComponent*> mPrevCollisionList;      // ���� ������ �浹 ����Ʈ
    std::list<CColliderComponent*> mCurrentCollisionList;   // ���� ������ �浹 ����Ʈ
    bool mbCheckCurrentSection;
    CollisionResult mResult;
    CollisionResult mMouseResult;
    std::list<std::function<void(const CollisionResult&)>> mCollisionCallBack[(int)eCollisionState::Max];
    std::list<std::function<void(const CollisionResult&)>> mCollisionMouseCallBack[(int)eCollisionState::Max];
    bool mbMouseCollision;

    // ����� ���� Mesh�� Shader
    CSharedPtr<class CMesh> mMesh;
    CSharedPtr<class CShader> mShader;

    // Material�� ������ ���� �ʿ� �����Ƿ�, ��� ���۸� ���� ������ �ִ´�.
    // Color�� ���̴��� �Ѱ��ִ� ��� ����
    class CColliderConstantBuffer* mCBuffer;
};

